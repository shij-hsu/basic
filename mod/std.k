
//Here is a list of prominent combinators
let I=\x.x
let K=\x.\y.x
let B=\f.\g.\x.f (g x)
let C=\f.\x.\y.f y x
let S=\f.\g.\x.f x (g x)
let w=\x.x x
let W=w w

//We encode pairs as follows
let pair=\x.\y.\f.f x y
let fst=\p.p(\x.\y.x)
let snd=\p.p(\x.\y.y)
//The following equivalences are easy to prove:
//      fst (pair x y)=x
//      snd (pair x y)=y

