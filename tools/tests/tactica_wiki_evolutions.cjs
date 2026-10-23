const assert = require("node:assert/strict");
const describe = require("../../docs/assets/pokedex-evolution.js");
for (const lang of ["fr", "en"]) {
  const c = {lang, itemName: x => ({ITEM_LINKING_CORD:"Fil Liaison", ITEM_DRAGON_SCALE:"Écaille Draco", ITEM_LEADERS_CREST:"Emblème du Général", ITEM_GIMMIGHOUL_COIN:"Pièce de Mordudor"}[x] || x),
    speciesName:x => ({SPECIES_BISHARP:"Scalproie",SPECIES_NINJASK:"Ninjask"}[x] || x),
    moveName:x => x === "MOVE_RAGE_FIST" ? "Poing de Colère" : x,
    conditionLabel:x => x === "IF_HOLD_ITEM" ? (lang === "fr" ? "objet tenu" : "held item") : x,
    conditionValue:x => c.itemName(x), humanConst:x=>x};
  const level = describe({method:"EVO_LEVEL",param:"0",conditions:[]},c);
  assert.equal(level,lang === "fr" ? "Monter d’un niveau" : "Level up");
  const held = describe({method:"EVO_ITEM",param:"ITEM_LINKING_CORD",conditions:[["IF_HOLD_ITEM","ITEM_DRAGON_SCALE"]]},c);
  assert.match(held,/Fil Liaison/); assert.match(held,/Écaille Draco/);
  const move = describe({method:"EVO_LEVEL",param:"0",conditions:[["IF_USED_MOVE_X_TIMES","MOVE_RAGE_FIST, 20"]]},c);
  assert.match(move,/Poing de Colère 20/); assert.doesNotMatch(move,/MOVE_|Niveau 0|Level 0/);
  const boss = describe({method:"EVO_LEVEL",param:"0",conditions:[["IF_DEFEAT_X_WITH_ITEMS","SPECIES_BISHARP, ITEM_LEADERS_CREST, 3"]]},c);
  assert.match(boss,/3 Scalproie/);assert.match(boss,/Emblème du Général/);
  const coins = describe({method:"EVO_LEVEL",param:"0",conditions:[["IF_BAG_ITEM_COUNT","ITEM_GIMMIGHOUL_COIN, 999"]]},c);
  assert.match(coins,/999 × Pièce de Mordudor/);
  const split = describe({method:"EVO_SPLIT_FROM_EVO",param:"SPECIES_NINJASK",conditions:[]},c);
  assert.match(split,/Ninjask/);assert.match(split,lang === "fr" ? /place libre/ : /free party slot/);
  const spin = describe({method:"EVO_SPIN",param:"SPIN_CCW_LONG",conditions:[]},c);
  assert.match(spin,lang === "fr" ? /antihoraire longue/ : /Long counterclockwise/);
}
console.log("Evolution rendering: FR/EN level-up, held item, compound conditions, split and spin passed");
