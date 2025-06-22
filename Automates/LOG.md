# Steps
npm init -y            # or pnpm init
npm i --save nearley   # 
npm i --save-dev nearley                      # + CLI locally
// or:  npm i -g nearley                      # CLI globally


## Compile grammar
npx nearleyc grammar.ne -o grammar.js


## Debug using `nearly-test`
npx nearley-test grammar.js -i "hi Bob"

