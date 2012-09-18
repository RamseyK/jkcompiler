program testFunctionCallObject;

class myCustomClass

BEGIN
   
   VAR
      lightSwitch	      : boolean;
      compilerWorks	      : boolean;

FUNCTION setCompilerWorks(value	: boolean ): integer;
BEGIN
   compilerWorks := value;
   setCompilerWorks := 4
END   
   
END



class testFunctionCallObject

BEGIN
   
   VAR
      onOff	     : boolean;
      myCustomObject : myCustomClass;
      retval	     : integer;

FUNCTION testFunctionCallObject;
BEGIN
      retval := myCustomObject.setCompilerWorks(onOff)
END

END
.
