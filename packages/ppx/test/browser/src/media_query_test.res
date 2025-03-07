open Jest

let width = "218px"
let media_type = "screen"

module Variables = {
  let media_type = "screen"
}

let testData = [
  (
    "(min-width: 30em)",
    %cx("@media (min-width: 30em) { color: brown; }"),
    CssJs.style(. [CssJs.media(. "(min-width: 30em) ", [CssJs.color(CssJs.brown)])]),
  ),
  (
    "(min-width: 30em) and (min-height: 200px)",
    %cx("@media (min-width: 30em) and (min-height: 200px) { color: brown; }"),
    CssJs.style(. [
      CssJs.media(. "(min-width: 30em) and (min-height: 200px) ", [CssJs.color(CssJs.brown)]),
    ]),
  ),
  (
    "@media(min-width: 0)",
    %cx("@media(min-width: 0) { color: brown; }"),
    CssJs.style(. [CssJs.media(. "(min-width: 0) ", [CssJs.color(CssJs.brown)])]),
  ),
  (
    "@media screen",
    %cx("@media screen { color: brown; }"),
    CssJs.style(. [CssJs.media(. "screen ", [CssJs.color(CssJs.brown)])]),
  ),
  (
    "@media screen",
    %cx("@media all and (max-width: 218px)
           and (max-height: 281px)
           and (orientation: portrait) { color: brown; }"),
    CssJs.style(. [
      CssJs.media(.
        "all and (max-width: 218px) and (max-height: 281px) and (orientation: portrait) ",
        [CssJs.color(CssJs.brown)],
      ),
    ]),
  ),
  (
    "@media with inteporlation",
    %cx("@media (max-width: $(width)) { color: brown }"),
    CssJs.style(. [CssJs.media(. "(max-width: 218px) ", [CssJs.color(CssJs.brown)])]),
  ),
  (
    "@media interpolation with and",
    %cx("@media only $(Variables.media_type) and (min-width: $(width)) { color: red }"),
    CssJs.style(. [CssJs.media(. "only screen and (min-width: 218px) ", [CssJs.color(CssJs.red)])]),
  ),
  (
    "@media support calc and interpolation",
    %cx("@media (max-width: calc( 20px * $(width))) { color: red }"),
    CssJs.style(. [CssJs.media(. "(max-width: calc(20px * 218px)) ", [CssJs.color(CssJs.red)])]),
  ),
  /* (
    "prefers-color-scheme: dark",
    [%cx "@media (prefers-color-scheme: dark) { color: white; }"],
    CssJs.style(. [|
      CssJs.media(
        "prefers-color-scheme: dark",
        [|CssJs.color(CssJs.white)|],
      ),
    |])
  ),
  (
    "screen and (prefers-reduced-motion: reduce",
    [%cx "@media screen and (prefers-reduced-motion: reduce) { color: white; }"],
    CssJs.style(. [|
      CssJs.media(
        "media screen and (prefers-reduced-motion: reduce)",
        [|CssJs.color(CssJs.white)|],
      ),
    |])
  ), */
]

describe("media-queries", _ =>
  Belt.Array.forEachWithIndex(testData, (index, (name, cssIn, emotionOut)) =>
    test(string_of_int(index) ++ (". Supports " ++ name), () =>
      Expect.expect(cssIn) |> Expect.toMatch(emotionOut)
    )
  )
)
