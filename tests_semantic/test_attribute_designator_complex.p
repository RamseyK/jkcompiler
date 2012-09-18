program testAttributeDesignatorComplex;

class main

BEGIN
   VAR aa, bb, cc, dd : integer;   
END


class testAttributeDesignatorComplex

BEGIN
   VAR object : main;

FUNCTION testAttributeDesignatorComplex;
BEGIN
   object.aa := object.bb + object.cc
END

END
.

