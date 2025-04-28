-- TODO: EVERYWHERE, pick a fuckin coding style, move shit to be in good places
-- TODO: Standardise "include" to be safe and usable in any .lua

function GetWorkingDirectory()
	local dir = os.getenv("PWD")
    if dir == nil then
        dir = io.popen("cd"):read("*a"):gsub("\n$", "")
    end
    
	return dir
end

function ModuleIncludeRoot()
    return GetWorkingDirectory()
end

function GetModulePath(moduleName, relativePath)
    if relativePath == nil or relativePath == "" then
        return ModuleIncludeRoot() .. "\\" .. moduleName
    end

    return ModuleIncludeRoot() .. "\\" .. relativePath .. "\\" .. moduleName
end

-- Core
local corePath = GetModulePath("Core")
print(corePath)
local Core = require(GetModulePath("Core"))
Core.Namespaces = {""}
local NameSpaceTable = Core.Namespaces
table.insert(Core, {Namespace={"Core"}})

function GetModule(moduleName, moduleRelativePath)
    local newModulePath = GetModulePath(moduleName, moduleRelativePath)

    if NameSpaceTable == nil then
        print("Bad Scope")
        return
    end

    if NameSpaceTable[moduleName] == nil then
        print("Namespace doesn't contain " .. moduleName)
    else
        print("Namespace " .. moduleName .. " already exists")
        return
    end

    table.insert(NameSpaceTable, moduleName)
    print("Added module " .. moduleName)
    return require(newModulePath)
end

function Core.GetWorkingDirectory()
	local dir = os.getenv("PWD")
    if dir == nil then
        dir = io.popen("cd"):read("*a"):gsub("\n$", "")
    end
    
	return dir
end

-- Includes


Core.Project = GetModule("Project")
Core.Utils = GetModule("Utils")

return Core