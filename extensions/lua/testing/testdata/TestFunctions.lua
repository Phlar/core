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

print("This should only be seen when executing the entire script.")