program testFunctionCallObjectExtend;

class myCustomClass

BEGIN
   
   VAR
      lightSwitch	      : boolean;
      compilerWorks	      : boolean;

FUNCTION setCompilerWorks(value	: boolean ): integer;
BEGIN
   compilerWorks := value;
   setCompilerWorks := 2
END   
   
END


class middleManClass extends myCustomClass

BEGIN
END   


class testFunctionCallObjectExtend

BEGIN
   
   VAR
      onOff	     : boolean;
      myCustomObject : middleManClass;
      retval	     : integer;
   
FUNCTION testFunctionCallObjectExtend;
BEGIN
      retval := myCustomObject.setCompilerWorks(onOff)
END

END
.
