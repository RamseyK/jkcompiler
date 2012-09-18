program testDataTypeDefined;

class myCustomClass
BEGIN
   VAR
      test : integer;
FUNCTION myCustomClass;
BEGIN
   test := 1
END   
END



class myCustomClass
BEGIN
   VAR
      test : integer;
FUNCTION myCustomClass;
BEGIN
   test := 2
END   
END



class testDataTypeDefined 
BEGIN
   VAR 
      lightSwitch	      : myCustomClass;
      compilerWorks	      : integer;

FUNCTION TestDataTypeDefined;
BEGIN
   compilerWorks := 1
END

END
.

