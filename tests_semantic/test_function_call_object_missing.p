program testFunctionCallObjectMissing;

class myCustomClass

BEGIN
   
   VAR
      lightSwitch	      : boolean;
END



class testFunctionCallObjectMissing

BEGIN
   
   VAR
      onOff	     : boolean;
      compilerWorks  : boolean;   
      myCustomObject : myCustomClass;

FUNCTION setCompilerWorks(value	: boolean ): integer;
BEGIN
   compilerWorks := value;
   setCompilerWorks := 9
END;   

FUNCTION testFunctionCallObjectMissing;
BEGIN
      onOff := myCustomObject.setCompilerWorks(onOff)
END

END
.
