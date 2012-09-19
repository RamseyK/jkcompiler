program testFunctionCallBase;

class baseClass

BEGIN
   
   VAR
      lightSwitch	      : boolean;
      compilerWorks	      : boolean;

FUNCTION setCompilerWorks(value	: boolean ): integer;
BEGIN
   compilerWorks := value;
   setCompilerWorks := 21
END   
   
END


class middleMan extends baseClass

BEGIN
   VAR confusing : boolean;
END		 



class testFunctionCallBase extends middleMan

BEGIN
   
   VAR
      onOff  : boolean;
      retval : integer;
   
FUNCTION testFunctionCallBase;
BEGIN
      retval := setCompilerWorks(onOff)
END

END
.
