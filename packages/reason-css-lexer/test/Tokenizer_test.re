open Setup;
open Reason_css_lexer;
open Location;

let parse = input => {
  let values =
    switch (from_string(input)) {
    | Ok(values) => values
    | Error(`Frozen) => failwith("Parser got frozen")
    };
  let {loc, _} = List.hd(values);
  let values = values |> List.map(({txt, _}) => txt);
  (loc, values);
};

let render_token =
  fun
  | EOF => ""
  | t => show_token(t);

let list_parse_tokens_to_string = tokens =>
  tokens
  |> List.rev
  |> List.map(
       fun
       | Ok(token) => render_token(token)
       | Error((token, err)) =>
         "Error(" ++ show_error(err) ++ ") " ++ show_token(token),
     )
  |> String.concat(" ")
  |> String.trim;

let list_tokens_to_string = tokens =>
  tokens |> List.map(render_token) |> String.concat(" ") |> String.trim;

describe("Tokenizer", ({test, _}) => {
  let success_tests_data = [
    (" \n\t ", [WHITESPACE], 4),
    ({|"something"|}, [STRING("something")], 11),
    // TODO: is that right?
    ({|#2|}, [HASH("2", `UNRESTRICTED)], 2),
    ({|#abc|}, [HASH("abc", `ID)], 4),
    ({|#|}, [DELIM("#")], 1),
    ({|'tuturu'|}, [STRING("tuturu")], 8),
    ({|(|}, [LEFT_PARENS], 1),
    ({|)|}, [RIGHT_PARENS], 1),
    ({|+12.3|}, [NUMBER(12.3)], 5),
    ({|+|}, [DELIM("+")], 1),
    ({|,|}, [COMMA], 1),
    ({|-45.6|}, [NUMBER(-45.6)], 5),
    ({|-->|}, [CDC], 3),
    ({|--potato|}, [IDENT("--potato")], 8),
    ({|-|}, [DELIM("-")], 1),
    ({|.7|}, [NUMBER(0.7)], 2),
    ({|.|}, [DELIM(".")], 1),
    ({|:|}, [COLON], 1),
    ({|;|}, [SEMICOLON], 1),
    ({|<!--|}, [CDO], 4),
    ({|<|}, [DELIM("<")], 1),
    ({|@mayushii|}, [AT_KEYWORD("mayushii")], 9),
    ({|@|}, [DELIM("@")], 1),
    ({|[|}, [LEFT_SQUARE], 1),
    ("\\@desu", [IDENT("@desu")], 6),
    ({|]|}, [RIGHT_SQUARE], 1),
    ({|12345678.9|}, [NUMBER(12345678.9)], 10),
    ({|bar|}, [IDENT("bar")], 3),
    ({||}, [EOF], 0),
    ({|!|}, [DELIM("!")], 1),
    ("1 / 1", [NUMBER(1.), WHITESPACE, DELIM("/"), WHITESPACE, NUMBER(1.)], 5),
    (
      {|calc(10px + 10px)|},
      [
        FUNCTION("calc"),
        DIMENSION(10., "px"),
        WHITESPACE,
        DELIM("+"),
        WHITESPACE,
        DIMENSION(10., "px"),
        RIGHT_PARENS,
      ],
      17,
    ),
    (
      {|calc(10px+ 10px)|},
      [
        FUNCTION("calc"),
        DIMENSION(10., "px"),
        DELIM("+"),
        WHITESPACE,
        DIMENSION(10., "px"),
        RIGHT_PARENS,
      ],
      16,
    ),
    ({|calc(10%)|}, [FUNCTION("calc"), PERCENTAGE(10.), RIGHT_PARENS], 9),
  ];

  success_tests_data
  |> List.iter(((input, output, last_position)) =>
       test(
         "should succed lexing: " ++ input,
         ({expect, _}) => {
           let (loc, values) = parse(input);
           let inputTokens = list_parse_tokens_to_string(values);
           let outputTokens = list_tokens_to_string(output);
           expect.string(inputTokens).toEqual(outputTokens);

           loc.loc_end.pos_cnum == last_position
             ? ()
             : failwith(
                 "position should be "
                 ++ string_of_int(last_position)
                 ++ " received "
                 ++ string_of_int(loc.loc_end.pos_cnum),
               );
         },
       )
     );
});
