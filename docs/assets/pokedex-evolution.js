/* Shared, DOM-independent description of the ROM's evolution records. */
((root) => {
"use strict";
function evolutionText(ev, c) {
  const fr = c.lang === "fr";
  let base;
  switch (ev.method) {
    case "EVO_LEVEL":
    case "EVO_LEVEL_BATTLE_ONLY":
      base = Number(ev.param) > 0 ? (fr ? "Niveau " : "Level ") + ev.param
        : (fr ? "Monter d’un niveau" : "Level up");
      if (ev.method === "EVO_LEVEL_BATTLE_ONLY") base += fr ? " pendant le combat" : " during battle";
      break;
    case "EVO_ITEM": base = (fr ? "Utiliser " : "Use ") + c.itemName(ev.param); break;
    case "EVO_BATTLE_END": base = fr ? "Après un combat" : "After a battle"; break;
    case "EVO_SCRIPT_TRIGGER": base = fr ? "Déclencheur spécifique dans Tactica" : "Specific Tactica trigger"; break;
    case "EVO_SPLIT_FROM_EVO":
      base = (fr ? "Lors de l’évolution en " : "When evolving into ") + c.speciesName(ev.param)
        + (fr ? ", avec une place libre dans l’équipe" : ", with a free party slot");
      break;
    case "EVO_SPIN": {
      const spins = {
        SPIN_CW_SHORT: ["Rotation horaire courte (< 5 s)", "Short clockwise spin (< 5 s)"],
        SPIN_CCW_SHORT: ["Rotation antihoraire courte (< 5 s)", "Short counterclockwise spin (< 5 s)"],
        SPIN_CW_LONG: ["Rotation horaire longue (5–9 s)", "Long clockwise spin (5–9 s)"],
        SPIN_CCW_LONG: ["Rotation antihoraire longue (5–9 s)", "Long counterclockwise spin (5–9 s)"],
        SPIN_EITHER: ["Rotation dans l’un des deux sens (au moins 10 s)", "Spin in either direction (at least 10 s)"]
      };
      base = spins[ev.param]?.[fr ? 0 : 1] || (fr ? "Rotation dans le monde" : "Overworld spin");
      break;
    }
    default: base = c.humanConst(ev.method, "EVO_");
  }
  const conditions = (ev.conditions || []).map(([key, value]) => {
    const args = (value || "").split(/\s*,\s*/);
    if (key === "IF_USED_MOVE_X_TIMES")
      return (fr ? "Utiliser " : "Use ") + c.moveName(args[0]) + " " + args[1] + (fr ? " fois" : " times");
    if (key === "IF_DEFEAT_X_WITH_ITEMS")
      return (fr ? "Vaincre " : "Defeat ") + args[2] + " " + c.speciesName(args[0])
        + (fr ? " tenant " : " holding ") + c.itemName(args[1]);
    if (key === "IF_BAG_ITEM_COUNT")
      return (fr ? "Avoir " : "Have ") + args[1] + " × " + c.itemName(args[0]) + (fr ? " dans le Sac" : " in the Bag");
    if (/^IF_ATK_(GT|EQ|LT)_DEF$/.test(key)) return c.conditionLabel(key);
    return c.conditionLabel(key) + (value ? ", " + c.conditionValue(value) : "");
  });
  return conditions.length ? base + " · " + conditions.join(" · ") : base;
}
root.TacticaEvolutionText = evolutionText;
if (typeof module !== "undefined" && module.exports) module.exports = evolutionText;
})(typeof window !== "undefined" ? window : globalThis);
