// Generated automatically by nearley, version 2.20.1
// http://github.com/Hardmath123/nearley
(function () {
function id(x) { return x[0]; }
var grammar = {
    Lexer: undefined,
    ParserRules: [
    {"name": "E", "symbols": ["T"]},
    {"name": "E", "symbols": ["E", {"literal":"+"}, "T"]},
    {"name": "T", "symbols": ["P"]},
    {"name": "T", "symbols": ["T", {"literal":"*"}, "P"]},
    {"name": "P", "symbols": [{"literal":"a"}]}
]
  , ParserStart: "E"
}
if (typeof module !== 'undefined'&& typeof module.exports !== 'undefined') {
   module.exports = grammar;
} else {
   window.grammar = grammar;
}
})();
