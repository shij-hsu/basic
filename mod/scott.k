
load std
let zero_=\x.\f.x
let succ_=\n.\x.\f.f n
let one_=succ_ zero_
let pred_=\n.n zero_ I

