firstTable = {
  hehe = 1,
  hoho = 2
}

secondTable = {}

local i = 1
for k, v in pairs(firstTable) do
  print(k)
  secondTable[i] = v
  i = i + 1
end

for k, v in pairs(secondTable) do
  print (k)
  print(v)
end