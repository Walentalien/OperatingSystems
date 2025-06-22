// Generated automatically by nearley, version 2.20.1
// http://github.com/Hardmath123/nearley
(function () {
function id(x) { return x[0]; }
var grammar = {
    Lexer: undefined,
    ParserRules: [
    {"name": "term", "symbols": ["number", {"literal":"+"}, "term"]},
    {"name": "term", "symbols": ["number"]},
    {"name": "number", "symbols": [/[0-9]/]}
]
  , ParserStart: "term"
}
if (typeof module !== 'undefined'&& typeof module.exports !== 'undefined') {
   module.exports = grammar;
} else {
   window.grammar = grammar;
}
})();
