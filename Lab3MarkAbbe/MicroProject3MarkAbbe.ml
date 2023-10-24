(* Parser for
   E -> T + E | T
   T -> A * T | A
   A -> [0-9]+ *)

#load "str.cma"
open Str;;

(***** Scanner *****)

type token =
  | Tok_Num of int
  | Tok_Sum 
  | Tok_Mul
  | Tok_END

let re_num = Str.regexp "[0-9]+"
let re_add = Str.regexp "+"
let re_mul = Str.regexp "\\*"

exception IllegalExpression of string

let tokenize str =
  let rec tok pos s =
    if pos >= String.length s then
      [Tok_END]
    else if (Str.string_match re_num s pos) then
      let token = Str.matched_string s in
      (Tok_Num (int_of_string token))::(tok (pos + (String.length token)) s)
    else if (Str.string_match re_add s pos) then
      Tok_Sum::(tok (pos+1) s)
    else if (Str.string_match re_mul s pos) then
      Tok_Mul::(tok (pos+1) s)
    else
      raise (IllegalExpression "tokenize")
  in
  tok 0 str

(***** Parser *****)

type exp = 
  | Num of int
  | Sum of exp * exp
  | Mul of exp * exp

let rec a_to_str a =
  match a with
  | Num n -> string_of_int n 
  | Sum (a1,a2) -> "(" ^ (a_to_str a1) ^ " + " ^ (a_to_str a2) ^ ")"
  | Mul (a1,a2) -> "(" ^ (a_to_str a1) ^ " * " ^ (a_to_str a2) ^ ")"
;;

let tok_list = ref []

exception ParseError of string

let lookahead () =
  match !tok_list with
  | [] -> raise (ParseError "no tokens")
  | (h::t) -> h

let match_tok a =
  match !tok_list with
  | (h::t) when a = h -> tok_list := t
  | _ -> raise (ParseError "bad match")

let rec parse_E () =
  let t1 = parse_T () in
  let t = lookahead () in
  match t with
  | Tok_Sum ->
    match_tok Tok_Sum;
    let t2 = parse_E () in
    Sum(t1,t2)
  | _ -> t1

and parse_T () =
  let a1 = parse_A () in
  let t = lookahead () in
  match t with
  | Tok_Mul ->
    match_tok Tok_Mul;
    let a2 = parse_T () in
    Mul(a1, a2)
  | _ -> a1

and parse_A () =
  let t = lookahead () in
  match t with
  | Tok_Num n ->
    let _ = match_tok (Tok_Num n) in
    Num n
  | _ -> raise (ParseError "parse_A")

let parse str =
  tok_list := (tokenize str);
  let exp = parse_E () in
  if !tok_list <> [Tok_END] then
    raise (ParseError "parse_E")
  else
    exp
;;

(***** Interpreter ****)

let rec eval a =
  match a with
  | Num n -> n
  | Sum (a1,a2) -> (eval a1) + (eval a2)
  | Mul (a1,a2) -> (eval a1) * (eval a2)
;;

let eval_str str =
  print_string str; print_string "\n";
  let e = parse str in
  print_string "AST produced = " ;
  print_endline (a_to_str e) ;
  let v = eval e in
  print_string "Value of AST = " ;
  print_int v ;
  print_endline "";
  v
;;

eval_str "1+2*3+4*5"
