PROGRAM ObjMethodCallNoParam;

CLASS Object
BEGIN
	VAR value : integer;

FUNCTION GetValue: integer;
BEGIN	
	GetValue := value
END
END

CLASS ObjMethodCallNoParam
BEGIN
	VAR obj : Object;
	    num : integer;

FUNCTION ObjMethodCallNoParam;
BEGIN
	obj := NEW Object;
	obj.value := 5;
	num := obj.GetValue
END

END
.