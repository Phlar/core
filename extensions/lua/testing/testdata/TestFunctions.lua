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

function FuncCallMethodOfCustomClass(testClass)
    testClass:MemberFunctionNoParam()
end

print("This should only be seen when executing the entire script.")