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

## Available Data types
> Here is a list of implemented data types:
* list
* int
* bool
* float
* string
* object
* ref

### List example
```
list colors = [
    "red",
    "green",
    "blue"
];
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
