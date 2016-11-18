
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

let plus=\n.\m.\f.\x.n f (m f x)
let mult=\n.\m.\f.\x.n (m f) x
