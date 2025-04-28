local Utils = {}

function Utils.ThisScriptDirectory()
    local str = debug.getinfo(2, "S").source:sub(2)
    return str:match("(.*[/\\])") or "./"
end

function Utils.ThisScriptName()
    local str = debug.getinfo(2, "S").source:sub(2)
    return str:match("^.*/(.*).lua$") or str
end

function Utils.ThisScriptFullPath()
    return Utils.ThisScriptDirectory() .. Utils.ThisScriptName() .. ".lua"
end

function Utils.PrintList(list, optID)
    Core.static_assert(type(list) == "table", "param expected to be table, received " .. type(list))
    
    local message = ""
    if optID ~= nil then
        Core.static_assert(type(optID) == "string")
        message = tostring(optID)
    end 
    local count = 0

    for i, v in ipairs(list) do
        message = message .. "\n" .. tostring(i) .. ":  " .. tostring(v)
        count = count + 1
    end

    if count == 0 then
        for i, v in pairs(list) do
            message = message .. "\n" .. tostring(i) .. ":  " .. tostring(v)
            count = count + 1
        end
    end
    
    message = message .. "\nCount: " .. tostring(count)
    print(message)
end

function GetTabs(count)
    local tabs = ""
    for i = 1, count do
        tabs = tabs .. "\t"
    end
    return tabs
end

function Utils.BuildTableTreeMessage(currentTable, depth, currentDepth)
    local message = ""

    for k, v in pairs(currentTable) do

        if type(k) == "string" then
            if type(v) == "table" then
                message = message .. "\n" .. GetTabs(currentDepth) .. tostring(k)
            else
                message = message .. "\n" .. GetTabs(currentDepth) .. tostring(k) .. ": " .. tostring(v)
            end
        else
            message = message .. "\n" .. GetTabs(currentDepth) .. tostring(v)
        end


        if currentDepth < depth and type(v) == "table" then
            message = message .. Utils.BuildTableTreeMessage(v, depth, currentDepth + 1)
        end
    end

    return message
end

-- Recursive Entry
function Utils.PrintTableTree(table, depth, optID, currentDepth)
    Core.static_assert(type(table) == "table", "param expected to be table, received " .. type(table))
    if currentDepth == nil then
        currentDepth = 0
    end
    if depth == nil then
        depth = 1
    end

    if optID == nil then
        optID = ""
    else
        Core.static_assert(type(optID) == "string")--
    end 

    local message = optID .. ":" .. Utils.BuildTableTreeMessage(table, depth+1, 1)
    print(message)
    
end

function Utils.FileExists(name)
    local f=io.open(name,"r")
    return f ~= nil
 end

return Utils