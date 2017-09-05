Utility = {}

Utility.splitStringFromComma = function(str)
	local finalStr = {}
	local index = 1
	for i in string.gmatch(str, "([^,]+)") do
		finalStr[index] = i
		index = index + 1
	end
	return finalStr
end


Utility.splitStringFromSpace = function(str)
    local tableStr = {}
    local index = 1
    for token in string.gmatch(str, "[^%s]+") do
        tableStr[index] = token
        index = index + 1
    end

    return tableStr
end

Utility.splitString = function(pString, pPattern)
   local Table = {}  -- NOTE: use {n = 0} in Lua-5.0
   local fpat = "(.-)" .. pPattern
   local last_end = 1
   local s, e, cap = pString:find(fpat, 1)
   while s do
      if s ~= 1 or cap ~= "" then
     table.insert(Table,cap)
      end
      last_end = e+1
      s, e, cap = pString:find(fpat, last_end)
   end
   if last_end <= #pString then
      cap = pString:sub(last_end)
      table.insert(Table, cap)
   end
   return Table
end

Utility.deepTableCopy = function (orig)
    local orig_type = type(orig)
    local copy
    if orig_type == 'table' then
        copy = {}
        for orig_key, orig_value in next, orig, nil do
            copy[Utility.deepTableCopy(orig_key)] = Utility.deepTableCopy(orig_value)
        end
        setmetatable(copy, Utility.deepTableCopy(getmetatable(orig)))
    else -- number, string, boolean, etc
        copy = orig
    end
    return copy
end

Utility.isBothIndexRelated = function(leftIndex, rightIndex) 
	return (leftIndex & rightIndex ~= 0)
end

Utility.isBothIndexNotRelated = function(leftIndex, rightIndex)
	return (leftIndex & rightIndex == 0)
end