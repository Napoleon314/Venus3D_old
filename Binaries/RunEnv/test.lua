print("test.lua is called")

main.a = 9

print(main.a,type(main.aaa))

function TestFunc()
	print("function test")
end

function TestAdd(a,b)
	return a + b, a, b
end
