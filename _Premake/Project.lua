Core = require("Core")
Utils = require("Utils")

Project = {}

Project.C_ProjectRelative_ChildrenProjectsPath = "ChildProjects"
Project.C_ProjectRelative_LuaPath = ""

-- Gen this?
Project.C_PremakeLUT = 
{
	project=project,
	location=location,
	files=files
}



function Project.GetProjectPath(projectDir, projectName)
	-- assert projectDir ends with projectName
	return projectDir .. "/" .. projectName
end

function Project.GetLuaProjectFileName(projectName)
	return projectName .. ".lua"
end

function Project.GetProjectLuaPathWithExtension(projectDir, projectName)
	return Project.GetProjectPath(projectDir, projectName).. "/" .. Project.C_ProjectRelative_LuaPath .. Project.GetLuaProjectFileName(projectName)
end

function Project.GetProjectLuaPath(projectDir, projectName)
	return Project.GetProjectPath(projectDir, projectName).. "/" .. Project.C_ProjectRelative_LuaPath .. projectName
end

function Project.GetChildProjectsDir(projectDir, projectName)
	return Project.GetProjectPath(projectDir, projectName) .. "/" .. Project.C_ProjectRelative_ChildrenProjectsPath
end

function Project.GetChildrenProjectNamesFullPath(fullPath)

    local command = 'dir /a:d /b "' .. fullPath .. '"'

    local handle = io.popen(command, "r")
    if not handle then
        error("Failed to execute command: " .. command)
    end

    local directories = {}
    for line in handle:lines() do
        if line and line:lower() ~= "file not found" then
            table.insert(directories, line)
        end
    end
    handle:close()

    -- if #directories == 0 then
    --     print("Warning: No project components found at " .. fullPath)
    -- end

    return directories
end

function Project.GetChildrenProjectNames(projectDir, projectName)

    local fullPath = Project.GetChildProjectsDir(projectDir, projectName)
	Project.GetChildrenProjectNamesFullPath(fullPath)
end

function Project.IsProjectLua(luaFullPath)
	local table = require(luaFullPath)
	if table == nil then
		return false
	end


	-- TODO: finish

	return true
end

function Project.IsProjectFolder(projectFolder, projectName)
	local luaFullPath = Project.GetProjectLuaPathWithExtension(projectFolder, projectName)

	local luaExists = Utils.FileExists(luaFullPath) -- this literally opens the file lol, none of this is performant

	return luaExists-- and Project.IsProjectLua(luaFullPath)
end


-- This is also what data is changeables
Project.ProjectData =
{
	--- The 3 expressions are in overload order Project<-Const<-Overrideable
	-- New items only added for this project
	ProjectOnlyPremakeExpression = {},

	-- Nothing upstream can modify set values
	ConstantPremakeExpression = {},

	-- Something upstream can modify existing items or add new items
	OptionalPremakeExpression = {},
}

function Project.EvaluatePremakeExpression(expression)

	-- Headers
	local projName = expression.project
	local loc = expression.location
	local kindVal = expression.kind
	local groupVal = expression.group

	if groupVal ~= nil then
		group(groupVal)
	end

	project(projName)
		location(loc)
		kind(kindVal)

	for k, v in pairs(expression) do
		local call = Project.C_PremakeLUT[k]
		if call ~= nil then
			call(v)
		end
	end	

	if groupVal ~= nil then
		group("")
	end
end

function Project.ProjectData.ProjectDataCreateFromPath(projectDir, projectName)
	local path = Project.GetProjectLuaPath(projectDir, projectName)
	local moduleTable = require(path)

	if moduleTable == nil then
		moduleTable = {}
		moduleTable.ProjectOnlyPremakeExpression = {}
	elseif moduleTable.ProjectOnlyPremakeExpression == nil  then
		moduleTable.ProjectOnlyPremakeExpression = {}
	end

	Core.Table.MergeFromRHS(moduleTable.ProjectOnlyPremakeExpression, 
	{
		project=(projectName),
		location=(Project.GetProjectPath(projectDir, projectName)),
		group=(projectName)
	})

	return moduleTable
end

function Project.ProjectData.Inherit(self, other)
	if other == nil then
		return
	end
	if self == nil then
		error("unexpected")
		return
	end

	Utils.PrintTableTree(self, 5, "Us")
	Utils.PrintTableTree(other, 5, "Inherit parent")

	if self.ConstantPremakeExpression == nil then
		self.ConstantPremakeExpression = other.ConstantPremakeExpression
	end

	if self.OptionalPremakeExpression == nil then
		self.OptionalPremakeExpression = other.OptionalPremakeExpression
	end

	-- Fuckde
	--Core.Table.MergeFromRHS_NewOnly(self.ConstantPremakeExpression, other.ConstantPremakeExpression)
	if other.ConstantPremakeExpression ~= nil then

		self.ConstantPremakeExpression = Core.Table.create_from_rhs_new_only(other.ConstantPremakeExpression, self.ConstantPremakeExpression)
	end
	
	if other.OptionalPremakeExpression ~= nil then
		self.OptionalPremakeExpression = Core.Table.create_from_rhs(other.OptionalPremakeExpression, self.OptionalPremakeExpression)
	end


	if other.ProjectOnlyPremakeExpression ~= nil then
		if self.ProjectOnlyPremakeExpression == nil then
			self.ProjectOnlyPremakeExpression = {}
		end
		self.ProjectOnlyPremakeExpression.group = other.ProjectOnlyPremakeExpression.project .. "/" .. self.ProjectOnlyPremakeExpression.project
	end
end

function Project.ProjectData.CreateRecursive(projectDir, projectName, parentProject, depth)
	
	-- outBranch contains side-by-side partial ProjectData
	local outBranch = {}
	if not Project.IsProjectFolder(projectDir, projectName)
	then
		return nil
	end

	local newData = Project.ProjectData.ProjectDataCreateFromPath(projectDir, projectName)
	Project.ProjectData.Inherit(newData, parentProject)


	table.insert(outBranch, newData)

	-- Find child projects (components)
	local childProjectsPath = Project.GetChildProjectsDir(projectDir, projectName)
	local childPrjNames = Project.GetChildrenProjectNamesFullPath(childProjectsPath)
	for i, childPrjName in ipairs(childPrjNames) do
		local newChild = Project.ProjectData.CreateRecursive(childProjectsPath, childPrjName, newData, depth + 1)
		
		if newChild ~= nil then
			for k, v in pairs(newChild) do
				table.insert(outBranch, v)
			end				
		end
	end

	return outBranch
end


Project.DiscoveryParamsDefault = 
{
	TypeKey = "DiscoveryParamsDefault",

	RootProjects = {""}, -- Enforce
	RootDirectory = "", -- Enforce
	RecursiveSearch = true
}

function Project.DiscoveryParams()
	return Core.deepCopyTable(Project.DiscoveryParamsDefault) 
end

function Project.DiscoverAll(discoveryParams, rootData)

	-- Core.static_assert(Core.Type.equal(Project.DiscoveryParamsDefault, discoveryParams), "Malformed parameter")
	local outDiscoveries = {}

	if discoveryParams.RecursiveSearch then
		for i, rootProject in ipairs(discoveryParams.RootProjects) do
			local branch = Project.ProjectData.CreateRecursive(discoveryParams.RootDirectory, rootProject, rootData, 0)

			if branch ~= nil then
				for k, v in pairs(branch) do
					table.insert(outDiscoveries, v)
				end				
			end
		end

	end

	-- static assert type ProjectData
	return outDiscoveries
end

function Project.ProjectData.CreateMergedExpressions(self)
	if self == nil then return {} end

	local outExpression = {}

	if self.OptionalPremakeExpression ~= nil then
		outExpression = self.OptionalPremakeExpression
	end

	if self.ConstantPremakeExpression ~= nil then
		Core.Table.MergeFromRHS(outExpression, self.ConstantPremakeExpression)
	end


	if self.ProjectOnlyPremakeExpression ~= nil then
		Core.Table.MergeFromRHS(outExpression, self.ProjectOnlyPremakeExpression)
	end

	return outExpression
end

function Project.Evaluate(discovered)
	--Utils.PrintTableTree(discovered, 6, "Found Projects")

	for i, element in pairs(discovered) do
		local expression = Project.ProjectData.CreateMergedExpressions(element)
		Utils.PrintTableTree(expression, 5)
		Project.EvaluatePremakeExpression(expression)
	end
end


return Project