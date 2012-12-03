PROGRAM ObjMethodCallParam;

CLASS Object
BEGIN

FUNCTION PrintValue(value : integer) : integer;
BEGIN	
	PRINT value;
	PrintValue := value
END
END

CLASS ObjMethodCallParam
BEGIN
	VAR obj : Object;
	    num : integer;

FUNCTION ObjMethodCallParam;
BEGIN
	obj := NEW Object;
	num := obj.PrintValue(5)
END

END
.