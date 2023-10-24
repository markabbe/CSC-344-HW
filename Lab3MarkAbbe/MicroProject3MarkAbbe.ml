(* Scanner Types *)
type token = 
  | Tok_Char of char 
  | Tok_OR
  | Tok_Q
  | Tok_LPAREN
  | Tok_RPAREN
  | Tok_END
;;

let tokenize str =
  let len = String.length str in
  let rec tok pos =
    if pos >= len then [Tok_END]
    else 
      match str.[pos] with
      | '|' -> Tok_OR :: tok (pos + 1)
      | '?' -> Tok_Q :: tok (pos + 1)
      | '(' -> Tok_LPAREN :: tok (pos + 1)
      | ')' -> Tok_RPAREN :: tok (pos + 1)
      | c -> (Tok_Char c) :: tok (pos + 1)
  in
  tok 0
;;

(* AST Types *)
type re = 
  | C of char
  | Concat of re * re
  | Optional of re
  | Alternation of re * re
;;

let tok_list = ref []

let lookahead () =
  match !tok_list with
  | [] -> failwith "no tokens"
  | (h::t) -> h

let consume () =
  match !tok_list with
  | [] -> failwith "no tokens to consume"
  | (h::t) -> tok_list := t

let rec parse_E () =
  let t = parse_T () in
  match lookahead () with
  | Tok_OR -> 
    consume (); 
    let e = parse_E () in
    Alternation(t, e)
  | _ -> t

and parse_T () =
  let f = parse_F () in
  match lookahead () with
  | Tok_Char _ | Tok_LPAREN -> 
    let t = parse_T () in
    Concat(f, t)
  | _ -> f

and parse_F () =
  let a = parse_A () in
  match lookahead () with
  | Tok_Q -> 
    consume (); 
    Optional(a)
  | _ -> a

and parse_A () =
  match lookahead () with
  | Tok_LPAREN -> 
    consume (); 
    let e = parse_E () in
    (match lookahead () with
     | Tok_RPAREN -> consume (); e
     | _ -> failwith "Expected closing parenthesis")
  | Tok_Char c -> 
    consume (); 
    C(c)
  | _ -> failwith "Expected char or open parenthesis"
;;

let parse str =
  tok_list := tokenize str;
  let res = parse_E () in
  if lookahead () != Tok_END then failwith "Unexpected tokens"
  else res
;;

let rec matches exp str =
  match exp, str with
  | C(c1), c2::_ when c1 = c2 || c1 = '.' -> [List.tl str]
  | Concat(e1, e2), _ -> 
      let tails = matches e1 str in
      List.flatten (List.map (matches e2) tails)
  | Optional(e), _ -> str :: (matches e str)
  | Alternation(e1, e2), _ -> (matches e1 str) @ (matches e2 str)
  | _, [] -> []
  | _, _ -> []
;;

let full_match exp str =
  let tails = matches exp (String.to_seq str |> List.of_seq) in
  List.exists (fun tail -> tail = []) tails
;;

(* Interface *)
let rec main () =
  print_endline "pattern?";
  let pattern = read_line () in
  if pattern = "" then exit 0;
  let ast = parse pattern in
  input_strings ast

and input_strings ast =
  print_endline "string?";
  let str = read_line () in
  if str = "" then main ()
  else
    (if full_match ast str then print_endline "match" else print_endline "no match";
    input_strings ast)

let () = main ()