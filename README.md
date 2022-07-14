# SPQR
SPQR esoteric programming language based on the latin language

## Usage

`./spqr <input-filename> <output-filename>`

## Example

### Compiling

`./spqr example.spqr example`

### Running

`./example`

### Output

`Carthago delenda est!` <br>
`Gallia est omnis divisa in partes tres.`

## Basic syntax

### Hello world

`imprimire("Salve mundi!")`

### Datatypes

Integer - `numerus` <br>
String - `verbum` <br>
Char - `littera` <br>
Boolean - `booleanus`<br>
Float - `fluito` <br>

### If-else statement

`numerus a = I` <br>
`numerus b = V` <br><br>
`si b == a:` <br>
`imprimire("a aequalis b")` <br>
`aliud:` <br>
`imprimire("a non aequalis b")` <br>
`finis` <br>

### While statement

`numerus a = nullus`<br> <br>
`dum a < V:` <br>
`a += I` <br>
`finis` <br>

### Switch statement

`numerus a = VII`<br> <br>
`electio a:` <br>
`casus VII:` <br>
`imprimire("a aequalis VII")` <br>
`finis` <br>

### For statement

`nam numerus a = nullus, a < X, a++:`<br>
`imprimire("Ave Caesar!")` <br> 
`finis` <br>