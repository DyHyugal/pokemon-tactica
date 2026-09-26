(()=>{
"use strict";
const S=()=>window.TacticaDexSpecies||{};
const L=()=>window.TacticaLocalization||{moves:{},abilities:{},items:{},species:{}};
const M=()=>window.TacticaDexMoves||{};
const A=()=>window.TacticaDexAbilities||{};
const LS=()=>window.TacticaDexLearnsets||{};
const TM=()=>window.TacticaDexTutorMoves||{};
const lang=document.documentElement.lang==="fr"?"fr":"en";
const t=lang==="fr"?{
  source:"Données issues de Pokémon Tactica",
  stats:"Statistiques de base", evo:"Lignée évolutive", evoHow:"Conditions d’évolution",
  abilities:"Talents", hidden:"Talent caché", regular:"Talent",
  levelMoves:"Capacités apprises par niveau", tutorMoves:"Capacités via le PNJ capacités / CT",
  tutorHint:"Compatibilité réellement utilisée par le PNJ Tactica. Les capacités déjà apprises par niveau sont masquées ici.",
  locations:"Localisations dans Tactica", allLocations:"Voir toutes les localisations",
  noLocation:"Aucune rencontre sauvage trouvée dans les tables publiées. Cette espèce peut être obtenue par évolution, cadeau ou autre mécanique.",
  level:"Niv.", move:"Capacité", type:"Type", category:"Cat.", power:"Puis.", accuracy:"Préc.", pp:"PP",
  physical:"Physique", special:"Spéciale", status:"Statut", evolutionMove:"Évolution",
  close:"Fermer", noEvolution:"Pas d’évolution supplémentaire dans les données de la ROM.",
  loading:"Chargement des localisations…", none:"—", total:"Total",
  locationCols:["Zone","Méthode","Moment","Niveaux"]
}:{
  source:"Data sourced from Pokémon Tactica",
  stats:"Base stats", evo:"Evolution line", evoHow:"Evolution conditions",
  abilities:"Abilities", hidden:"Hidden Ability", regular:"Ability",
  levelMoves:"Level-up moves", tutorMoves:"Move NPC / TM compatibility",
  tutorHint:"Compatibility actually used by Tactica's move NPC. Moves already learned by level are hidden here.",
  locations:"Locations in Tactica", allLocations:"See all locations",
  noLocation:"No wild encounter was found in the published tables. This species may be obtained by evolution, gift or another mechanic.",
  level:"Lv.", move:"Move", type:"Type", category:"Cat.", power:"Power", accuracy:"Acc.", pp:"PP",
  physical:"Physical", special:"Special", status:"Status", evolutionMove:"Evolution",
  close:"Close", noEvolution:"No further evolution in the ROM data.",
  loading:"Loading locations…", none:"—", total:"Total",
  locationCols:["Area","Method","Time","Levels"]
};
const typeFR={NORMAL:"Normal",FIGHTING:"Combat",FLYING:"Vol",POISON:"Poison",GROUND:"Sol",ROCK:"Roche",BUG:"Insecte",GHOST:"Spectre",STEEL:"Acier",FIRE:"Feu",WATER:"Eau",GRASS:"Plante",ELECTRIC:"Électrik",PSYCHIC:"Psy",ICE:"Glace",DRAGON:"Dragon",DARK:"Ténèbres",FAIRY:"Fée",STELLAR:"Stellaire"};
const conditionFR={IF_MIN_FRIENDSHIP:"bonheur élevé",IF_GENDER:"genre",IF_TIME:"moment de la journée",IF_NOT_TIME:"hors d’un moment donné",IF_MIN_LEVEL:"niveau minimum",IF_HOLD_ITEM:"objet tenu",IF_KNOWS_MOVE:"capacité connue",IF_KNOWS_MOVE_TYPE:"type de capacité connu",IF_REGION:"région",IF_NOT_REGION:"hors région",IF_IN_MAPSEC:"dans la zone",IF_IN_MAP:"sur la carte",IF_WEATHER:"météo",IF_SPECIES_IN_PARTY:"espèce dans l’équipe",IF_TYPE_IN_PARTY:"type dans l’équipe",IF_TRADE_PARTNER_SPECIES:"partenaire d’échange",IF_MIN_BEAUTY:"Beauté minimum",IF_MIN_OVERWORLD_STEPS:"pas effectués",IF_CRITICAL_HITS_GE:"coups critiques minimum",IF_RECOIL_DAMAGE_GE:"dégâts de recul minimum",IF_CURRENT_DAMAGE_GE:"dégâts subis minimum",IF_USED_MOVE_X_TIMES:"utilisations d’une capacité",IF_DEFEAT_X_WITH_ITEMS:"adversaires vaincus avec objet",IF_BAG_ITEM_COUNT:"quantité d’objet dans le Sac",IF_PID_MODULO_100_EQ:"condition interne de forme",IF_PID_MODULO_100_GT:"condition interne de forme",IF_PID_UPPER_MODULO_10_GT:"condition interne de forme",IF_PID_UPPER_MODULO_10_LT:"condition interne de forme",IF_ATK_GT_DEF:"Attaque > Défense",IF_ATK_EQ_DEF:"Attaque = Défense",IF_ATK_LT_DEF:"Attaque < Défense"};
const conditionEN={IF_MIN_FRIENDSHIP:"high friendship",IF_GENDER:"gender",IF_TIME:"time of day",IF_NOT_TIME:"outside a given time",IF_MIN_LEVEL:"minimum level",IF_HOLD_ITEM:"held item",IF_KNOWS_MOVE:"known move",IF_KNOWS_MOVE_TYPE:"known move type",IF_REGION:"region",IF_NOT_REGION:"outside region",IF_IN_MAPSEC:"in area",IF_IN_MAP:"on map",IF_WEATHER:"weather",IF_SPECIES_IN_PARTY:"species in party",IF_TYPE_IN_PARTY:"type in party",IF_TRADE_PARTNER_SPECIES:"trade partner",IF_MIN_BEAUTY:"minimum Beauty",IF_MIN_OVERWORLD_STEPS:"overworld steps",IF_CRITICAL_HITS_GE:"minimum critical hits",IF_RECOIL_DAMAGE_GE:"minimum recoil damage",IF_CURRENT_DAMAGE_GE:"minimum damage taken",IF_USED_MOVE_X_TIMES:"move uses",IF_DEFEAT_X_WITH_ITEMS:"foes defeated with item",IF_BAG_ITEM_COUNT:"item quantity in Bag",IF_PID_MODULO_100_EQ:"internal form condition",IF_PID_MODULO_100_GT:"internal form condition",IF_PID_UPPER_MODULO_10_GT:"internal form condition",IF_PID_UPPER_MODULO_10_LT:"internal form condition",IF_ATK_GT_DEF:"Attack > Defense",IF_ATK_EQ_DEF:"Attack = Defense",IF_ATK_LT_DEF:"Attack < Defense"};
const norm=v=>(v||"").normalize("NFD").replace(/[\u0300-\u036f]/g,"").replace(/[^a-z0-9♀♂]+/gi,"").toLowerCase();
const title=v=>(v||"").toLowerCase().replace(/(^|[\s-])([a-zà-ÿ])/g,(m,a,b)=>a+b.toUpperCase());
const humanConst=(v,prefix="")=>title((v||"").replace(prefix,"").replaceAll("_"," "));
const spriteSlugFromConst=c=>(c||"").replace(/^SPECIES_/,"").toLowerCase().replaceAll("_","-")
  .replace(/-alola$/,"-alola").replace(/-galar$/,"-galar").replace(/-hisui$/,"-hisui").replace(/-paldea$/,"-paldea");
const localize=(group,en)=>lang==="fr"?(L()[group]?.[(en||"").toLowerCase()]||en):en;
const typeName=x=>lang==="fr"?(typeFR[x]||humanConst(x)):humanConst(x);
const moveInfo=token=>{
  const raw=M()[token]||{};
  const en=raw.name?title(raw.name):humanConst(token);
  return {...raw,en,name:localize("moves",en)};
};
const abilityInfo=token=>{
  const raw=A()[token]||{};
  const en=raw.name||humanConst(token);
  return {...raw,en,name:localize("abilities",en)};
};
const itemName=token=>{
  const en=humanConst(token,"ITEM_");
  return localize("items",en);
};
const speciesLabel=(constant)=>{
  const mapped=displayByConst.get(constant);
  if(mapped)return mapped;
  const d=S()[constant]; if(!d)return humanConst(constant,"SPECIES_");
  const en=title(d.name);
  if(lang==="fr") return L().species?.[en.toLowerCase()]||en;
  return en;
};
let allCards=[...document.querySelectorAll(".dex-card")];
const constByCard=new Map(), displayByConst=new Map();
function resolveCard(card){
  if(constByCard.has(card))return constByCard.get(card);
  const img=card.querySelector(".poke-sprite");
  const slug=img?decodeURIComponent(img.src.split("/").pop().replace(/\.png(?:\?.*)?$/,"")):"";
  const slugN=norm(slug), name=card.querySelector("strong")?.textContent||"", nameN=norm(name);
  let best=null,bestScore=-1;
  for(const [c,d] of Object.entries(S())){
    const cN=norm(c.replace(/^SPECIES_/,"")), runtimeN=norm(d.name);
    let score=0;
    if(slugN&&cN===slugN)score+=1000;
    if(nameN&&runtimeN===nameN)score+=200;
    if(slugN&&slugN.startsWith(runtimeN))score+=80;
    if(cN===nameN)score+=100;
    const toks=c.replace(/^SPECIES_/,"").toLowerCase().split("_");
    for(const tok of toks.slice(1))if(tok.length>1&&slugN.includes(norm(tok)))score+=4;
    if(score>bestScore){best=c;bestScore=score;}
  }
  if(best){constByCard.set(card,best);displayByConst.set(best,name);}
  return best;
}
allCards.forEach(resolveCard);

function ensureEvolutionFamilyCards(){
  const dex=document.getElementById("dex");
  if(!dex)return;
  const edges=[];
  for(const [src,d] of Object.entries(S()))
    for(const ev of d.evolutions||[])
      if(S()[ev.target])edges.push([src,ev.target]);
  const wanted=new Set([...constByCard.values()]);
  let changed=true;
  while(changed){
    changed=false;
    for(const [a,b] of edges){
      if(wanted.has(a)&&!wanted.has(b)){wanted.add(b);changed=true;}
      if(wanted.has(b)&&!wanted.has(a)){wanted.add(a);changed=true;}
    }
  }
  const existing=new Set([...constByCard.values()]);
  for(const constant of wanted){
    if(existing.has(constant))continue;
    const d=S()[constant];
    if(!d)continue;
    const en=title(d.name);
    const display=lang==="fr"?(L().species?.[en.toLowerCase()]||en):en;
    const other=lang==="fr"?en:(L().species?.[en.toLowerCase()]||en);
    const card=document.createElement("article");
    card.className="card dex-card";
    card.dataset.search=(display+" "+other+" "+en).toLowerCase();
    const loc=(lang==="fr"?"Localisations.html?q=":"Locations.html?q=")+encodeURIComponent(display);
    card.innerHTML='<span class="poke-name"><img class="poke-sprite" loading="lazy" src="https://play.pokemonshowdown.com/sprites/gen5/'+spriteSlugFromConst(constant)+'.png" alt="'+display+'" onerror="this.style.display=\'none\'"><strong>'+display+'</strong></span><small>'+(lang==="fr"?"Fiche ROM":"ROM entry")+'</small><a class="card-link" href="'+loc+'">'+t.allLocations+' →</a>';
    dex.append(card);
    existing.add(constant);
  }
  allCards=[...document.querySelectorAll(".dex-card")];
  allCards.sort((a,b)=>(a.querySelector("strong")?.textContent||"").localeCompare((b.querySelector("strong")?.textContent||""),lang,{sensitivity:"base"}));
  allCards.forEach(card=>dex.append(card));
  allCards.forEach(resolveCard);
}
ensureEvolutionFamilyCards();

function bindDexSearch(){
  const q=document.getElementById("q"), count=document.getElementById("count");
  if(!q||!count)return;
  const apply=()=>{
    const term=norm(q.value);
    let n=0;
    allCards.forEach(card=>{
      const ok=!term||norm(card.dataset.search||"").includes(term);
      card.hidden=!ok;
      if(ok)n++;
    });
    count.textContent=n+" Pokémon";
  };
  q.addEventListener("input",apply);
  apply();
}
bindDexSearch();

let dialog=document.getElementById("dex-detail");
if(!dialog){
  dialog=document.createElement("dialog");
  dialog.id="dex-detail"; dialog.className="dex-detail";
  dialog.innerHTML='<div class="dex-detail-shell"><button class="dex-close" type="button" aria-label="'+t.close+'">×</button><div id="dex-detail-content"></div></div>';
  document.body.append(dialog);
}
dialog.querySelector(".dex-close").addEventListener("click",()=>dialog.close());
dialog.addEventListener("click",e=>{if(e.target===dialog)dialog.close()});
const content=dialog.querySelector("#dex-detail-content");
let locationsPromise=null;
function locationDocument(){
  if(!locationsPromise){
    const url=lang==="fr"?"Localisations.html":"Locations.html";
    locationsPromise=fetch(url).then(r=>r.text()).then(x=>new DOMParser().parseFromString(x,"text/html")).catch(()=>null);
  }
  return locationsPromise;
}
function statRows(d){
  const labels=lang==="fr"?[["hp","PV"],["atk","Attaque"],["def","Défense"],["spa","Att. Spé."],["spd","Déf. Spé."],["spe","Vitesse"]]:[["hp","HP"],["atk","Attack"],["def","Defense"],["spa","Sp. Atk"],["spd","Sp. Def"],["spe","Speed"]];
  const total=labels.reduce((n,[k])=>n+(d.stats[k]||0),0);
  return labels.map(([k,label])=>'<div class="stat-row"><span>'+label+'</span><strong>'+d.stats[k]+'</strong><i><b style="width:'+Math.min(100,(d.stats[k]||0)/2.55)+'%"></b></i></div>').join("")+
    '<div class="stat-row stat-total"><span>'+t.total+'</span><strong>'+total+'</strong><i></i></div>';
}
function conditionValue(v){
  if(!v)return "";
  if(v.startsWith("ITEM_"))return itemName(v);
  if(v.startsWith("MOVE_"))return moveInfo(v.slice(5)).name;
  if(v.startsWith("TYPE_"))return typeName(v.slice(5));
  if(v.startsWith("SPECIES_"))return speciesLabel(v);
  if(v.startsWith("MAPSEC_"))return humanConst(v,"MAPSEC_");
  if(v.startsWith("MAP_"))return humanConst(v,"MAP_");
  if(v.startsWith("WEATHER_"))return humanConst(v,"WEATHER_");
  if(v==="MON_MALE")return lang==="fr"?"mâle":"male";
  if(v==="MON_FEMALE")return lang==="fr"?"femelle":"female";
  if(v==="TIME_DAY")return lang==="fr"?"jour":"day";
  if(v==="TIME_NIGHT")return lang==="fr"?"nuit":"night";
  if(v==="FRIENDSHIP_EVO_THRESHOLD")return lang==="fr"?"seuil d’amitié":"friendship threshold";
  return humanConst(v);
}
function evoText(ev){
  let base="";
  if(ev.method==="EVO_LEVEL"||ev.method==="EVO_LEVEL_BATTLE_ONLY"){
    base=(lang==="fr"?"Niveau ":"Level ")+(ev.param||"?");
  }else if(ev.method==="EVO_ITEM"){
    base=(lang==="fr"?"Utiliser ":"Use ")+itemName(ev.param);
  }else if(ev.method==="EVO_TRADE"){
    base=lang==="fr"?"Condition d’évolution Tactica (ancien échange)":"Tactica evolution condition (former trade)";
  }else if(ev.method==="EVO_BATTLE_END"){
    base=lang==="fr"?"Après un combat":"After a battle";
  }else if(ev.method==="EVO_SCRIPT_TRIGGER"){
    base=lang==="fr"?"Déclencheur spécifique dans Tactica":"Specific Tactica trigger";
  }else if(ev.method==="EVO_SPIN"){
    base=lang==="fr"?"Rotation dans le monde":"Overworld spin";
  }else base=humanConst(ev.method,"EVO_");
  const cond=(ev.conditions||[]).map(([k,v])=>{
    const lbl=(lang==="fr"?conditionFR:conditionEN)[k]||humanConst(k,"IF_");
    return lbl+(v?", "+conditionValue(v):"");
  });
  return cond.length?base+" · "+cond.join(" · "):base;
}
function componentEdges(start){
  const edges=[];
  for(const [src,d] of Object.entries(S()))for(const ev of d.evolutions||[])if(S()[ev.target])edges.push({src,...ev});
  const seen=new Set([start]);let changed=true;
  while(changed){changed=false;for(const e of edges){if(seen.has(e.src)&&!seen.has(e.target)){seen.add(e.target);changed=true}if(seen.has(e.target)&&!seen.has(e.src)){seen.add(e.src);changed=true}}}
  return edges.filter(e=>seen.has(e.src)&&seen.has(e.target));
}
function evoHtml(constant){
  const edges=componentEdges(constant);
  if(!edges.length)return '<p class="muted">'+t.noEvolution+'</p>';
  const nodes=new Set(); edges.forEach(e=>{nodes.add(e.src);nodes.add(e.target)});
  const predecessors=new Set(edges.map(e=>e.target));
  let roots=[...nodes].filter(n=>!predecessors.has(n)); if(!roots.length)roots=[constant];
  const depth=new Map(roots.map(r=>[r,0]));let changed=true;
  while(changed){changed=false;for(const e of edges){if(depth.has(e.src)&&(!depth.has(e.target)||depth.get(e.target)>depth.get(e.src)+1)){depth.set(e.target,depth.get(e.src)+1);changed=true}}}
  const nodeHtml=[...nodes].sort((a,b)=>(depth.get(a)??99)-(depth.get(b)??99)).map(n=>'<button type="button" class="evo-node'+(n===constant?" current":"")+'" data-open-species="'+n+'"><img src="https://play.pokemonshowdown.com/sprites/gen5/'+spriteSlugFromConst(n)+'.png" alt="" onerror="this.style.display=\'none\'"><span>'+speciesLabel(n)+'</span></button>').join("");
  const edgeHtml=edges.map(e=>'<div class="evo-rule"><button type="button" data-open-species="'+e.src+'">'+speciesLabel(e.src)+'</button><span>→ <strong>'+evoText(e)+'</strong> →</span><button type="button" data-open-species="'+e.target+'">'+speciesLabel(e.target)+'</button></div>').join("");
  return '<div class="evo-nodes">'+nodeHtml+'</div><div class="evo-rules">'+edgeHtml+'</div>';
}
function abilitiesHtml(d){
  const abs=(d.abilities||[]).map((a,i)=>({a,i})).filter(x=>x.a);
  if(!abs.length)return '<p>'+t.none+'</p>';
  return '<div class="ability-grid">'+abs.map(({a,i})=>{
    const hidden=i===2;
    const ai=abilityInfo(a);
    return '<div class="ability-card"><small>'+(hidden?t.hidden:t.regular)+'</small><strong>'+ai.name+'</strong>'+(ai.description?'<span class="ability-description">'+ai.description+'</span>':'')+'</div>';
  }).join("")+'</div>';
}
function moveTable(list){
  if(!list.length)return '<p>'+t.none+'</p>';
  return '<div class="table-wrap dex-moves-wrap"><table class="dex-moves"><thead><tr><th>'+t.level+'</th><th>'+t.move+'</th><th>'+t.type+'</th><th>'+t.category+'</th><th>'+t.power+'</th><th>'+t.accuracy+'</th><th>'+t.pp+'</th></tr></thead><tbody>'+
    list.map(([lvl,token])=>{const m=moveInfo(token);const cat=m.category==="PHYSICAL"?t.physical:m.category==="SPECIAL"?t.special:t.status;return '<tr><td>'+(lvl===0?t.evolutionMove:lvl)+'</td><td><strong>'+m.name+'</strong></td><td><span class="type">'+typeName(m.type)+'</span></td><td>'+cat+'</td><td>'+(m.power||t.none)+'</td><td>'+(m.accuracy?m.accuracy+"%":t.none)+'</td><td>'+(m.pp??t.none)+'</td></tr>'}).join("")+
    '</tbody></table></div>';
}
function tutorHtml(constant,levelMoves){
  const learned=new Set(levelMoves.map(x=>x[1]));
  const list=(TM()[constant]||[]).filter(x=>!learned.has(x));
  if(!list.length)return '<p>'+t.none+'</p>';
  return '<details class="dex-tutor"><summary>'+t.tutorMoves+' <span>'+list.length+'</span></summary><p class="muted">'+t.tutorHint+'</p><div class="move-chip-grid">'+list.map(token=>{const m=moveInfo(token);return '<span class="move-chip"><strong>'+m.name+'</strong><small>'+typeName(m.type)+' · '+(m.category==="PHYSICAL"?t.physical:m.category==="SPECIAL"?t.special:t.status)+'</small></span>'}).join("")+'</div></details>';
}
async function fillLocations(constant,displayName,linkHref){
  const box=content.querySelector("#dex-locations");
  if(!box)return;
  box.innerHTML='<p class="muted">'+t.loading+'</p>';
  const doc=await locationDocument(); if(!doc){box.innerHTML='<p>'+t.noLocation+'</p>';return;}
  const d=S()[constant], candidates=[displayName,d?.name||"",speciesLabel(constant)].map(norm).filter(Boolean);
  const rows=[...doc.querySelectorAll("#loc tbody tr")].filter(r=>{
    const slots=r.children[4]?.textContent||"";
    return slots.split("·").some(slot=>{
      const species=norm(slot.replace(/\s*\d+\s*%\s*$/,"").trim());
      return candidates.some(candidate=>species===candidate);
    });
  }).slice(0,10);
  if(!rows.length){box.innerHTML='<p>'+t.noLocation+'</p><a class="detail-location-link" href="'+linkHref+'">'+t.allLocations+' →</a>';return;}
  box.innerHTML='<div class="location-mini-grid">'+rows.map(r=>{const td=[...r.children];return '<div class="location-mini"><strong>'+td[0].textContent+'</strong><span>'+td[1].textContent+' · '+td[2].textContent+'</span><small>'+td[3].textContent+' · '+td[4].textContent+'</small></div>'}).join("")+'</div><a class="detail-location-link" href="'+linkHref+'">'+t.allLocations+' →</a>';
}
function openSpecies(constant,originCard=null){
  const d=S()[constant]; if(!d)return;
  const display=originCard?.querySelector("strong")?.textContent||speciesLabel(constant);
  const sprite=originCard?.querySelector(".poke-sprite")?.src||("https://play.pokemonshowdown.com/sprites/gen5/"+spriteSlugFromConst(constant)+".png");
  const levelMoves=d.learnset?(LS()[d.learnset]||[]):[];
  const q=encodeURIComponent(display);
  const locHref=(lang==="fr"?"Localisations.html?q=":"Locations.html?q=")+q;
  content.innerHTML='<div class="dex-detail-hero"><img class="dex-detail-sprite" src="'+sprite+'" alt="'+display+'" onerror="this.style.display=\'none\'"><div><span class="eyebrow">'+t.source+'</span><h2>'+display+'</h2><div class="type-list">'+(d.types||[]).map(x=>'<span class="type">'+typeName(x)+'</span>').join("")+'</div></div></div>'+
    '<div class="dex-detail-grid"><section><h3>'+t.stats+'</h3><div class="stats-panel">'+statRows(d)+'</div></section><section><h3>'+t.abilities+'</h3>'+abilitiesHtml(d)+'</section></div>'+
    '<section><h3>'+t.evo+'</h3>'+evoHtml(constant)+'</section>'+
    '<section><h3>'+t.levelMoves+'</h3>'+moveTable(levelMoves)+'</section>'+
    '<section>'+tutorHtml(constant,levelMoves)+'</section>'+
    '<section><h3>'+t.locations+'</h3><div id="dex-locations"></div></section>';
  content.querySelectorAll("[data-open-species]").forEach(b=>b.addEventListener("click",()=>openSpecies(b.dataset.openSpecies,null)));
  fillLocations(constant,display,locHref);
  if(!dialog.open)dialog.showModal();
}
allCards.forEach(card=>{
  card.classList.add("dex-card-interactive");
  card.tabIndex=0; card.setAttribute("role","button");
  card.setAttribute("aria-label",(lang==="fr"?"Ouvrir la fiche de ":"Open details for ")+(card.querySelector("strong")?.textContent||"Pokémon"));
  const direct=card.querySelector(".card-link");
  if(direct)direct.addEventListener("click",e=>e.stopPropagation());
  card.addEventListener("click",e=>{if(e.target.closest(".card-link"))return;const c=resolveCard(card);if(c)openSpecies(c,card)});
  card.addEventListener("keydown",e=>{if((e.key==="Enter"||e.key===" ")&&!e.target.closest(".card-link")){e.preventDefault();const c=resolveCard(card);if(c)openSpecies(c,card)}});
});
})();
