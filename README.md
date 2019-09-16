# Hermes
> A C-like scripting language.

## Example program
> Here is a small program that iterates and prints the contents of a list:
```C
list fruits = [
    "apple",
    "banana",
    "pear",
    "lemon"
];

int i = 0;

while (i < fruits.length)
{
    print(fruits[i]);
    i += 1;
}
```

## Linking a C program
> Hermes can execute functions written in C.
> To load a function written in C, you can use the `dload` method, to load
> a function from a shared object file (`.so`). Example:
```C
dload("librequests.so", "httpget");

string response = httpget("http://example.org")

print(response)
```
> Here, the `httpget` function was loaded from the `librequests.so` file.
> Read more about how to write C methods for Hermes [here](DLOAD.md).

## Available Data types
> Here is a list of implemented data types:
* list
* int
* bool
* float
* string
* object
* ref
* source

### List example
```
list colors = [
    "red",
    "green",
    "blue"
];
```

### Built-in list methods
#### Add
> To add an item to a list:
```C
list names = [];

names.add("john");
```
### Remove
> To remove an item from a list by index
```
list names = ["john"];

names.remove(0);
```

### Int example
> Everyone knows what an integer is.
```C
int age = 22;
```

### Bool example
> Everyone knows what an boolean is.
```C
bool x = 10 > 3;
```

### Float example
> Everyone knows what an float is.
```C
float x = 0.5;
```

### String example
> Everyone knows what a string is.
```C
string name = "John Doe";
```

### Object example
> Objects are sort of what you think they are.
```C
object person = {
    string name = "john";
    int age = 22;
};

print(person.name);
```

### Ref example
> Refs are supposed to be used when integrating hermes in some sort of environment and you want to keep track of "references" that the programmer can work with.
```C
ref something;

something.x += 1;
```

### Source example
> Sources are basically objects that represents a parsed source code.
```C
source s = include("examples/functions.he");
```
> To have hermes interpret the source, simply use the built-in `visit` method:
```C
visit(s);
```
> Now you can also dump that source to a serialized `.dat` file using the built-in `wad` method:
```C
wad(s, "functions");
```
> This will create a `functions.dat` file.
> To read the use case for these `.dat` files, please [read this](DAT.md).

## Built-in methods
* print
* pprint
* include
* wad
* lad
* visit

### print
> Prints what ever you gives it, example:
```
print("hello world");
```

### pprint
> Prints the adress of a value, example:
```
object person = {string name = "John Doe";};

pprint(person);
```

### include
> Loads an external source file, example:
```
source s = include("examples/functions.he");
```

### wad
> Writes an AST compound to disk, example:
```
source s = include("examples/functions.he");
wad(s, "functions");
```
> This creates a `functions.dat` file.

### lad
> Loads an AST compound from disk, example:
```
source s = lad("functions");
```

### visit
> Visits and executes a source, example:
```
source s = include("examples/functions.he");
visit(s);
```

## Available statements
* new

### new example
```C
object get_person(string name)
{
    object o = {
        string name;    
    };

    o.name = name;

    return o;
}

object person = new get_person("Hanna");
```
> The `new` statement will always return a new address of whatever is to the
> right of the statement.

## Compositions
> Hermes now also support compositions, like this:
```
int add_2(int x)
{
    return x + 2;
}

int remove_1(int x)
{
    return x - 1;
}

int mycomp(int x) =
    add_2, remove_1;

int x = mycomp(10);

print(x);
```

## Notes
### For loops
> For loops does not exist, you can acheive the same thing with while loops and we are trying to keep the language simple.

### Lists
> This might not be obvious, but lists can contain any sort of value. Example:
```C
list cool_stuff = [
    "this is a string",
    { string x = "Wow, this is an object"; },
    [
        "a string in a list in a list"
    ]
];
```

## Installing Hermes
> To install Hermes on your system, simple run:
```bash
make && sudo make install
```
