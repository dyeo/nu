print: imp sys.print

object: fn(id, name)
{{
    @id: id ; @ is equivalent to this or self in other languages
    @name: name
    @print: fn {
        print("{@id}: {@name}")
    }
}}

a: object(0, "First")
a.print()

b: object(1, "Second")
b.print()