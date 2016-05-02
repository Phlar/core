function FuncNoParam()
end

function FuncCallSingleFreeFunction()
    FreeFunctionNoParamA()
end

function FuncCallTwoFreeFunctions()
    FreeFunctionNoParamA()
    FreeFunctionNoParamB()
end

function FuncCallTwoSimpleParameters(a, b)
    sum = a + b
    print("\na: " .. a .. "\nb: " .. b .. "\nsum: " .. sum)
end

function FuncCallMethodOfCustomClassNoParam(testClass)
    testClass:MemberFunctionNoParam()
end

function FuncCallMethodOfCustomClass(testClass)
    testClass:MemberFunction(testClass)
end

function FuncReturnNothing()
end

function FuncReturnBool()
    return true
end

function FuncReturnString()
    return "foo"
end

function FuncReturnInt()
    return 42
end

function FuncReturnFloat()
    return 123.456
end

print("This should only be seen when executing the entire script.")