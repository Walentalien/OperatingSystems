// Generated automatically by nearley, version 2.20.1
// http://github.com/Hardmath123/nearley
(function () {
function id(x) { return x[0]; }

  // JS optinal
var grammar = {
    Lexer: undefined,
    ParserRules: [
    {"name": "main", "symbols": ["greeting", {"literal":" "}, "name"]},
    {"name": "greeting$string$1", "symbols": [{"literal":"h"}, {"literal":"e"}, {"literal":"l"}, {"literal":"l"}, {"literal":"o"}], "postprocess": function joiner(d) {return d.join('');}},
    {"name": "greeting", "symbols": ["greeting$string$1"]},
    {"name": "greeting$string$2", "symbols": [{"literal":"h"}, {"literal":"i"}], "postprocess": function joiner(d) {return d.join('');}},
    {"name": "greeting", "symbols": ["greeting$string$2"]},
    {"name": "name$ebnf$1", "symbols": [/[a-zA-Z]/]},
    {"name": "name$ebnf$1", "symbols": ["name$ebnf$1", /[a-zA-Z]/], "postprocess": function arrpush(d) {return d[0].concat([d[1]]);}},
    {"name": "name", "symbols": ["name$ebnf$1"]}
]
  , ParserStart: "main"
}
if (typeof module !== 'undefined'&& typeof module.exports !== 'undefined') {
   module.exports = grammar;
} else {
   window.grammar = grammar;
}
})();
