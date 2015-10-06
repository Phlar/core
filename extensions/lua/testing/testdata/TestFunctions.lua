function FuncNoParam()
end

function FuncCallSingleFreeFunction()
    FreeFunctionNoParamA()
end

function FuncCallTwoFreeFunctions()
    FreeFunctionNoParamA()
    FreeFunctionNoParamB()
end

print("This should only be seen when executing the entire script.")