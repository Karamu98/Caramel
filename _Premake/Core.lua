local Core = {}

-- TODO: Move to global namespace
function Core.static_assert(condition, message)
    if not condition then
        local message = "Static Assertion Failed: " .. (message or "No details provided")
        error(debug.traceback(message, 2))
    end
end

-- TODO: Move to Table.
function Core.deepCopyTable(original)
    local originalType = type(original)

    if originalType ~= "table" then
        local message = "Core.deepCopyTable : parameter was type (" .. originalType .. "). Expects table. Call stack:"
        error(debug.traceback(message, 2))
        return {}
    end

    local copy = {}
    for key, value in pairs(original) do
        if type(value) == "table" then
            copy[key] = Core.deepCopyTable(value) -- Recursively copy nested tables
        else
            copy[key] = value
        end
    end
    return copy
end

function Core.CopyDeep(object)
    if type(object) == "table" then
        return Core.deepCopyTable(object)
    end

    local new = object
    return new
end



-- Core.Type = {}
-- function Core.Type.has(object)
--     if type(object) == "function" then
--         Core.static_assert(false, "Object is a function")
--         return false
--     end

--     if object.TypeKey == nil then
--         Core.static_assert(false, "Object has no TypeKey field")
--         return false
--     end

--     local str = tostring(object.TypeKey)
--     if str == nil or str == "" then
--         Core.static_assert(false, "Object TypeKey field empty")
--         return false
--     end

--     return true
-- end

-- function Core.Type.equal(object1, object2)
--     if object1 == nil or object2 == nil then
--         return false
--     end
--     if Core.Type.has(object1) and Core.Type.has(object2) then
--         return object1.TypeKey == object2.TypeKey
--     end

--     return false
-- end


Core.Table = {}
-- Ignores table elements that aren't part of target already

function Core.Table.create_from_rhs(lhs, other)
    -- Core.static_assert(Core.Type.equal(lhs, other), "Malformed parameter")

    local outputVal = Core.CopyDeep(lhs)
    for key, value in pairs(other) do
        outputVal[key] = value
    end
    return outputVal
end

function Core.Table.MergeFromRHS(self, rhs)
    for key, value in pairs(rhs) do
        self[key] = Core.CopyDeep(value)
    end
end

function Core.Table.MergeFromRHS_NewOnly(self, rhs)
    for key, value in pairs(rhs) do
        if self[key] == nil then
            self[key] = Core.CopyDeep(value)     
        end
    end
end

function Core.Table.MergeFromRHS_ChangesOnly(self, rhs)
    for key, value in pairs(rhs) do
        if self[key] ~= nil then
            self[key] = Core.CopyDeep(value)     
        end
    end
end

function Core.Table.create_from_rhs_new_only(lhs, other)
    -- Core.static_assert(Core.Type.equal(lhs, other), "Malformed parameter")

    local outputVal = Core.deepCopyTable(lhs)
    for key, value in pairs(other) do
        if outputVal[key] == nil then
            outputVal[key] = Core.CopyDeep(value)
        end
    end

    return outputVal
end

function Core.Table.CreateFromRHSNewOnlyNoChecks(lhs, other)
    local outputVal = Core.deepCopyTable(lhs)
    for key, value in pairs(other) do
        if outputVal[key] == nil then
            outputVal[key] = Core.CopyDeep(value)
        end
    end

    return outputVal
end

function Core.Table.create_from_rhs_changes_only(lhs, other)
    -- Core.static_assert(Core.Type.equal(lhs, other), "Malformed parameter")

    local outputVal = Core.CopyDeep(lhs)
    for key, value in pairs(other) do
        if lhs[key] ~= nil then
            lhs[key] = Core.CopyDeep(value)
        end
    end
    return outputVal
end

function Core.Table.ApplyOverrides(target, overrides, mode)
    -- Core.static_assert(Core.Type.equal(target, overrides), "Malformed parameter")

    if mode == nil then
        mode = Core.Table.ApplyOverridesModes[0]
    end

    local output = Core.CopyDeep(target)
    
    for key, value in pairs(overrides) do
        if output[key] ~= nil then
            output[key] = Core.CopyDeep(value)
        end
    end


    return output
end

function Core.Table.contains_any_leaf(self, other)
    for key, _ in pairs(self) do
        if other[key] ~= nil then
            return true
        end
    end
    return false
end

function Core.Table.contains_any_non_leaf(self, other)
    for key, _ in pairs(other) do
        if self[key] == nil then
            return true
        end
    end
    return false
end













-- Project Common Structure



--- 
---  ==============================================
--- 				*** Component Project ***
--- 
-- 			"	WHERE THE FOLLOWING IS TRUE IN PATH 
--- 				"/Components/NameX/NameX.lua"
---  		Create a new Component Project called "NameX"  "
--- 
--- 
--- This project is best thought of as a discreet part of your overall
--- projects code, a module in collection etc

-- Definitions


-- TODO: Specify API header location/Accessor





return Core
