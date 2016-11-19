
//This is a standard module for Church Numbers.
//Church encoded a natural number n as a function
//iterating a given function n-times:
//          0:=\f.\x.x
//          1:=\f.\x.f x
//          ...
//          n:=\f.\x.f^n x
//Note that the Church numerals are normal combinators.
//This ensures that numerals for different numbers are
//not beta-equivalent (Church-Rosser property).
//

let zero=\f.\x.x
let succ=\n.\f.\x.f (n f x)
let one=succ zero
let two=succ one
let three=succ two
let four=succ three

let plus=\n.\m.\f.\x.n f (m f x)
let mult=\n.\m.\f.\x.n (m f) x


let zero=\f.\x.x
let succ=\n.\f.\x.f (n f x)
let one=succ zero
let two=succ one
let three=succ two
let four=succ three
let five=succ four
let six=succ five
let seven=succ six
let eight=succ seven
let nine=succ eight
let ten=succ nine

let plus=\n.\m.\f.\x.n f (m f x)
let mult=\n.\m.\f.\x.n (m f) x
let exp=\n.\m.m (mult n) one


