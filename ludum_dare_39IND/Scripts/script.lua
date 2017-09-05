dofile("script2.lua")
testString = "Lua"
number = 42


table = {}
print(table[1] == nil)

print(10 / 2.0 == 5.0)

for i = 0, 10, 2 do print(i) end

function abc(hehe, haha)
  print(hehe)
  print(haha)
end

abc(4)

local test = {}
test.a = 0
--test.__index = test

function test:print2()
  --self:bastard()
  self.a = self.a + 5
end

function test:print1()
  print(self)
  print(test)
  self:print2()
  self.a = self.a + 10
  print(self.a)
end

test:print1()


fuckingyea = {}

print(fuckingyea)

tablehehe = {"Albert"}
print(tablehehe[1])

os.execute ("PAUSE") 