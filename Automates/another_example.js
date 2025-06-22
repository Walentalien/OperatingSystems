// Generated automatically by nearley, version 2.20.1
// http://github.com/Hardmath123/nearley
(function () {
function id(x) { return x[0]; }
var grammar = {
    Lexer: undefined,
    ParserRules: [
    {"name": "S", "symbols": ["F"]},
    {"name": "F", "symbols": ["G"]},
    {"name": "F", "symbols": ["F", {"literal":"&"}, "G"]},
    {"name": "G", "symbols": ["H"]},
    {"name": "G", "symbols": ["G", {"literal":"^"}, "H"]},
    {"name": "H", "symbols": [{"literal":"x"}]}
]
  , ParserStart: "S"
}
if (typeof module !== 'undefined'&& typeof module.exports !== 'undefined') {
   module.exports = grammar;
} else {
   window.grammar = grammar;
}
})();
