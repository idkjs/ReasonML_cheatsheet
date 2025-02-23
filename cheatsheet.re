open Core;

let () = print_endline("hello world");

let variables_example = {
  /* ------------------------------------------------------------
     Variables
     ------------------------------------------------------------ */
  let x = 5;
  let real = 9. /* float (no trailing zero required) */;
  let s = "a string";

  /* ------------------------------------------------------------
     Annotated variables
     ------------------------------------------------------------ */
  let y: int = (6: int);
  let z: int = 7;

  /* ------------------------------------------------------------
     Annotated return type - [calc] returns an int
     ------------------------------------------------------------ */
  let calc = (x, y, s): int => {
    printf("A %s has %i lives\n", s, x + y);
    x + y;
  };

  /* ------------------------------------------------------------
     Renaming arguments
     ------------------------------------------------------------ */
  let f = (~a as renamed_a) => renamed_a * 2;
  let f = (~a as renamed_and_annotated: int) => f(~a=renamed_and_annotated);

  /* ------------------------------------------------------------
     Printing to the screen
     ------------------------------------------------------------ */
  printf("%d %d %d %f %s %i\n", x, y, z, real, s, f(~a=50));
  print_s([%message "Description"(s: string)]);
  print_s(String.sexp_of_t(s));
  /* This uses: https://github.com/janestreet/ppx_custom_printf*/
  printf("%{sexp:string} %{sexp#mach:string} %{String}\n"^, s, s, s);
  let _: int = calc(5, 4, "cat");

  /* ------------------------------------------------------------
     Recursion
     ------------------------------------------------------------ */
  let rec fold = (list, ~init, ~accum) =>
    switch (list) {
    | [] => init
    | [hd, ...tl] => fold(tl, ~init=accum(init, hd), ~accum)
    };

  /* ------------------------------------------------------------
     Anonymous function (fun keyword)
     ------------------------------------------------------------ */
  printf(
    "%i\n",
    fold([1, 2, 3], ~init=0, ~accum=(init, elem) => init + elem),
  );
};

/* ------------------------------------------------------------
   Records - Construction, destruction, annotation and renaming
   ------------------------------------------------------------ */
[@deriving sexp]
type contact = {
  name: string,
  mobile: string,
  birth_year: int,
};

let records_example = {
  /* record construction */
  let c = {name: "Adam", mobile: "012345678", birth_year: 1995};
  printf("%s %s %i\n", c.name, c.mobile, c.birth_year);
  printf("%{sexp:contact}\n"^, c);
  let create_contact = (name, mobile, birth_year) => {
    name,
    mobile,
    birth_year,
  };
  let c2 = create_contact("James", "999999999", 1988);
  /* Use dot notation */
  let print1 = x => printf("%s %s %i\n", x.name, x.mobile, x.birth_year);
  /* Use record deconstruction */
  let print2 = ({name, mobile, birth_year} /* much better */) =>
    printf("%s %s %i\n", name, mobile, birth_year);

  /* use argument renaming and annotated variable */
  let print3 = ({name, mobile, birth_year: b}: contact) =>
    printf("%s %s %i\n", name, mobile, b);
  let print4 = c => {
    /* use record deconstruction */
    let {name: n, mobile: m, birth_year} = c;
    printf("%s %s %i\n", n, m, birth_year);
  };

  print1(c);
  print2(c);
  print3(c);
  print4(c2);
};

/* ------------------------------------------------------------
   Record Types - records (types) in modules
   ------------------------------------------------------------ */
module File = {
  [@deriving sexp]
  type t = {
    file_name: string,
    size: int,
    attributes: int,
  };

  let to_string = t =>
    if (t.attributes > 700) {
      String.uppercase(t.file_name);
    } else {
      t.file_name;
    };
};

let record_types_example1 = {
  let create_directory_entry = (file_name, size, attributes) => {
    File.file_name,
    size,
    attributes,
  };
  let is_small_file = file => {
    let {File.file_name: _, size, attributes: _} = file;
    size < 1000;
  };

  let file = create_directory_entry("temp.log", 100, 755);
  printf("%{sexp:File.t}\n"^, file);
  /* ppx uses File.to_string */
  printf("%{File} : is_small_file=%b\n"^, file, is_small_file(file));
};

/* ------------------------------------------------------------
   Record Types - 2nd example
   ------------------------------------------------------------ */
module Price = {
  type t = {x: float};

  let create = x => {x: x};
};

let record_types_example2 = {
  let price1 = Price.create(5.50);
  let price2: Price.t = {x: 6.50};
  (ignore(price1), price2);
};

/* ------------------------------------------------------------
   Default arguments, named parameters and optional arguments
   ------------------------------------------------------------ */
let arguments_example = {
  /* by is optional but resolves to type int (not opional int)*/
  let increment = (~by=1, x) => x + by;
  let inc = (~by=?, x) =>
    switch (by) {
    | None => x + 1
    | Some(by) => x + by
    };
  let incr = (~by, x) => x + by;
  printf("0 inc = %d %d %d\n", increment(0), inc(0), incr(0, ~by=1));
  printf(
    "0 inc by 2 = %d %d %d %d\n",
    increment(0, ~by=2),
    inc(0, ~by=2),
    inc(0, ~by=?Some(2)),
    incr(0, ~by=2),
  );
};

/* ------------------------------------------------------------
   Tuples/Pairs (often better to use a record though)
   Open modules
   ------------------------------------------------------------ */
let tuples_example = {
  let gps_position = (latitude, longitude) => (latitude, longitude);
  let sydney = gps_position(-33.865143, 151.209900);
  let google_maps = gps =>
    Float.(
      "https://www.google.com/maps/search/"
      ++ to_string(fst(gps))
      ++ ","
      ++ to_string(snd(gps))
      ++ "/"
    );

  printf("Google Maps URL for Sydney is %s\n", google_maps(sydney));
};

/* ------------------------------------------------------------

   ------------------------------------------------------------ */
/* TODO:
      Google Analytics and Home Page
      (* TODO Add more*)

      [x] record deconstruction
      [x] record construction
      [ ] destructive substitution
      [ ]
      [ ]
      [ ]
      [x] records (022)
      [x] modules (021 023)
      [x] default arguments and optional arguments
      [x] tuples
      [ ] matching and function
      [ ] variants (004 024 025)
      [ ] polymorphic variants
      [ ] fun
      [ ] partial functions
      [ ] options
      [ ] ref and mutable
      [ ] List - map, fold, reduce (010 017 029 030  )
      [ ] Set
      [ ] Map
      [ ] Hashtbl (027)
      [ ] string (018 019)
      [ ] map + bind
      [ ] pipes
      [ ] In_channel (008)
      [ ] Lazy (028)
      [ ] Sexp (031)
      [ ] Identifiable
      [ ] functors (031)
      [ ] phantom types
      [ ] compare (020)
      [ ] async
      [ ] error handling and Or_error and Result.t (026)
      [ ] expect tests (002)
      [ ] Assembler & profiling (014 015 016)
      [ ] ppx
      [ ] Cool tricks
      [ ] Spacemacs commands
   */
