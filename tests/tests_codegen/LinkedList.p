PROGRAM LinkedList;

CLASS Node
BEGIN
VAR value : integer;
    next : Node;
END

CLASS LinkedList
BEGIN

VAR list, eol : Node;

FUNCTION LinkedList;
BEGIN
	list := NEW Node;
	list.value := 0;
	list.next := NEW Node;
	list.next.value := 5;
	list.next.next := NEW Node;
	list.next.next.value := 42;
	list.next.next.next := eol;
	WHILE list <> eol DO
	BEGIN
		PRINT list.value;
		list := list.next
	END
END

END
.
