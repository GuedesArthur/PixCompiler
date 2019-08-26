# PixCompiler 
Final project for Compilers class 2019.2

Pix compiler is maybe the first compiler to work with pixels rather than plain text. All you have to do is set a custom
pallet (or use the default one), open any image editor software, such as MS Paint or Paint.NET(Recommended) and start coding!

It's syntax is similar to FORTRAN's...but with pixels instead.

***Work in Progress***


**HOW TO CODE**

Firstly, run the compiler executable file. It will then create a pallet bitmap file and a text file.
The text file can be used to import the pallet on Paint.NET to make coding easier. Feel free to edit the pallet 
file with the colors of your choice, just making sure not to include two identical colors on it. 
Each color in the pallet will represent, respectively, the following keywords (colorwords, if you will):

    IF,     THEN,   ELSE,   WHILE,
    ENDLINE, END,
    INT,    LONG,   FLOAT,  STRING,
    TYPEINT,TYPELONG,TYPEFLOAT,TYPESTRING,
    IN,     OUT,    ASSIGN,
    ADD,    SUB,    MULT,   DIV,
    QUOT,   PAROPEN,PARCLOSE,
    AND,    OR,     NOT,
    EQUALS, DIFFERS,
    GREATER,GREATEREQUAL,
    LESS,   LESSEQUAL,
    INIT,   RETURN,
    ENDOFCODE, ID

Should the compiler find any color not present in the pallet, it will assume it's an identifier or, if preceeded 
by type keywords (INT, LONG, FLOAT), a value.

Then, with an editing software of your choice, paint the "colorwords" on your image and finally save it as 24-bit Color Bitmap.
After saving it, just drag the bitmap onto the compiler executable file and it will convert your pix code to a C language file,
which will be saved in the compiler's current directory.

**COLORWORD SYNTAX**

Here is a simple pix code, converted to text, showing its syntax.


    INIT

    TYPEFLOAT ID1 ENDLINE
    IN ID1 ENDLINE

    IF ID1 LESSEQUAL INT 5
    THEN OUT STRING QUOT M E N O R QUOT ENDLINE END
    ELSE OUT STRING QUOT M A I O R QUOT ENDLINE END

    RETURN INT 0

    ENDOFCODE

As said before, it's pretty similar to FORTRAN and C language. Just like both languages, it's weakly-typed.

**Declarations**

To declare a variable, just use the TYPE_VALUE keywords:

    TYPEINT ID1 ENDLINE
    TYPEFLOAT ID1 ENDLINE

Make sure to end the sentence with an ENDLINE keyword.

**Conditional**

Conditional structures follow a simple pattern:

    IF VALUE
    THEN ... END
    ELSE ... END 

The value structure represents the result of any operation between variables and constants. The if-clause verifies
if the value of VALUE is positive (DIFFERS 0) or negative (EQUALS 0). In the former case, it will follow the proceedures
contained between THEN and END. In the latter, it will follow the proceedures between ELSE and END (if present).

**Loops**

Loops follow a similar pattern:
    
    WHILE VALUE
    THEN ... END
        
Should VALUE be positive, the proceedures between THEN and END will repeat until it assumes a negative value.

**Assigning values**

The ASSIGN clause should be used like this:

    ASSIGN ID TYPE_VALUE VALUE ENDLINE

Just make sure that the type of the value matches its ID's type.

**I/O**

I/O structures should be used following the syntax below:

    IN ID ENDLINE
    OUT ID ENDLINE
    OUT INT VALUE ENDLINE

Again, just make sure that the type of the value matches its ID's type.



    
