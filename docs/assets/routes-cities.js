(()=>{"use strict";
const fr=document.documentElement.lang==="fr";
const cities=[
["New Bark Town","Bourg Geon","city",["Laboratoire du Prof. Orme","Maison du joueur"],["Choisir le premier starter au laboratoire.","Après M. Pokémon puis la séquence du vol/police, revenir au laboratoire : l'Œuf Mystère permet de reconnaître l'espèce et de choisir le second starter."],["Sélecteur 30 starters + Évoli","Second starter via l'Œuf Mystère"],[]],
["Cherrygrove City","Ville Griotte","city",["Centre Pokémon","Boutique","Maison du Guide"],["La première visite propose la visite guidée de la ville.","Au retour de chez M. Pokémon, le rival intercepte le joueur : premier combat Rival."],["Carte via le guide","Eau Mystique donnée par le pêcheur"],[]],
["Violet City","Mauville","city",["Arène d'Albert","Tour Chétiflor","Académie Pokémon","Centre Pokémon"],["Préparer puis affronter Albert.","L'assistant d'Orme attend au Centre Pokémon dans la progression liée à l'œuf."],["Badge Zéphyr","Étape de la quête de l'œuf"],[]],
["Azalea Town","Écorcia","city",["Arène d'Hector","Maison de Fargas","Puits Ramoloss"],["Résoudre l'incident du Puits Ramoloss lié à la Team Rocket.","Affronter Hector pour poursuivre vers le Bois aux Chênes."],["Services de Fargas"],[]],
["Goldenrod City","Doublonville","city",["Arène de Blanche","Tour Radio","Grand Magasin","Souterrain","Boutique de vélos","Fleuriste","Casino","Évaluateur de noms"],["La ville concentre plusieurs étapes de progression, puis l'arc de prise de contrôle de la Tour Radio par la Team Rocket.","L'Arène de Blanche constitue le jalon principal de la première visite."],["Vélo","Carapuce à O","Évoli","Objets du Grand Magasin","Move Tutor"],["Un échange PNJ est présent dans les scripts de la ville."]],
["Ecruteak City","Rosalia","city",["Arène de Mortimer","Tour Cendrée","Tours de Rosalia"],["Explorer la Tour Cendrée et avancer dans l'arc local.","Affronter Mortimer ; sa progression Tactica introduit les mécaniques Méga prévues."],["Progression Méga / Mega Ring selon jalon Tactica"],[]],
["Olivine City","Oliville","city",["Arène de Jasmine","Phare","Port"],["La progression mène au Phare puis vers Irisia avant le combat complet contre Jasmine."],[],[]],
["Cianwood City","Irisia","city",["Arène de Chuck","Pharmacie"],["Récupérer ce qui est nécessaire à l'arc du Phare puis affronter Chuck."],[],[]],
["Mahogany Town","Acajou","city",["Arène de Frédo","Accès Lac Colère","Repaire Rocket"],["Enquêter sur le Lac Colère puis le repaire de la Team Rocket.","Affronter Frédo dans la progression de Johto."],[],[]],
["Blackthorn City","Ébènelle","city",["Arène de Sandra","Antre du Dragon"],["Dernier grand jalon de badges de Johto avant la Ligue."],[],[]],
["Pallet Town","Bourg Palette","city",[],["Point de passage de Kanto dans la seconde partie de l'aventure."],[],[]],
["Viridian City","Jadielle","city",["Arène de Jadielle"],["Progression Kanto et accès aux routes voisines."],[],[]],
["Pewter City","Argenta","city",["Arène de Pierre"],["Affronter Pierre dans la progression Kanto."],[],[]],
["Cerulean City","Azuria","city",["Arène d'Ondine"],["Affronter Ondine et explorer les routes au nord."],[],[]],
["Vermilion City","Carmin sur Mer","city",["Arène du Major Bob"],["Affronter le Major Bob dans la progression Kanto."],[],[]],
["Lavender Town","Lavanville","city",[],["Carrefour de progression de Kanto."],[],[]],
["Celadon City","Céladopole","city",["Arène d'Erika"],["Affronter Erika et profiter des nombreux services de la ville."],[],[]],
["Fuchsia City","Parmanie","city",["Arène de Jeannine"],["Affronter Jeannine ; accès aux zones de capture associées à Parmanie."],[],[]],
["Saffron City","Safrania","city",["Arène de Morgane"],["Affronter Morgane et utiliser Safrania comme carrefour de Kanto."],[],[]],
["Cinnabar Island","Cramois'Île","city",[],["Étape de progression vers le combat associé à Auguste."],[],[]]
];
const routeNotes={
29:{story:["Première route entre Bourg Geon et Ville Griotte.","Tutoriel de capture optionnel après le début de l'aventure."],items:["Potion au sol","Mouchoir Soie via Tuscany (membre de la famille des jours)"]},
30:{story:["Progression vers la maison de M. Pokémon puis retour vers Bourg Geon."],items:[]},
31:{story:["Approche de Mauville ; accès aux zones voisines avant le premier badge."],items:[]},
32:{story:["Route de progression après Mauville vers le sud de Johto."],items:[]},
33:{story:["Courte liaison vers Écorcia et le Puits Ramoloss."],items:[]},
34:{story:["Liaison Bois aux Chênes → Doublonville."],items:[]},
35:{story:["Sortie nord de Doublonville vers le Parc Naturel."],items:[]},
36:{story:["Accessible très tôt par Mauville ; Tactica conserve ici des rencontres calibrées sur ce premier accès réel (niv. 14–17)."],items:[]},
37:{story:["Dernière liaison vers Rosalia."],items:[]},
38:{story:["Départ ouest de Rosalia vers Oliville."],items:[]},
39:{story:["Approche d'Oliville."],items:[]},
40:{story:["Départ maritime depuis Oliville vers Irisia."],items:[]},
41:{story:["Zone maritime des Tourb'Îles entre Oliville et Irisia."],items:[]},
42:{story:["Axe Rosalia–Acajou, avec accès au Mont Creuset."],items:[]},
43:{story:["Liaison Acajou–Lac Colère, importante dans l'arc Team Rocket."],items:[]},
44:{story:["Route est d'Acajou menant vers la Route de Glace."],items:[]},
45:{story:["Descente montagneuse depuis Ébènelle."],items:[]},
46:{story:["Route de retour vers le sud de Johto, connectée au secteur de la Route 29."],items:[]},
47:{story:["Progression vers l'ouest de Johto et les zones optionnelles."],items:[]},
48:{story:["Prolongement occidental de la Route 47."],items:[]}
};
for(let n=1;n<=50;n++){const rn="Route "+n,note=routeNotes[n]||{story:[],items:[]};cities.push([rn,rn,"route",[],note.story,note.items,[]]);}
const names={story:fr?"Histoire":"Story",interest:fr?"Points d’intérêt":"Points of interest",loot:fr?"À récupérer":"Things to collect",special:fr?"Échanges / cadeaux":"Trades / gifts",enc:fr?"Rencontres":"Encounters",none:fr?"Aucune information spécifique vérifiée dans les scripts Tactica pour le moment.":"No specific information verified in Tactica scripts yet.",see:fr?"Voir les localisations":"See locations",open:fr?"Ouvrir la fiche":"Open entry"};
const display=x=>fr?x[1]:x[0];
const grid=document.getElementById("place-grid"),dialog=document.getElementById("place-detail"),content=document.getElementById("place-detail-content");
let active="all",encDoc=null;
function card(x){const tags=[x[2],x[4].length?"story":"",x[3].join(" "),x[4].join(" "),x[5].join(" "),x[6].join(" ")].join(" ").toLowerCase();return '<article class="place-card" data-kind="'+x[2]+'" data-tags="'+tags+'" tabindex="0"><span class="place-kind">'+(x[2]==="city"?(fr?"Ville":"City"):(fr?"Route":"Route"))+'</span><h2>'+display(x)+'</h2><small>'+(x[3].slice(0,3).join(" · ")||(x[4][0]||"Pokémon Tactica"))+'</small><button type="button">'+names.open+' →</button></article>'}
function render(){const q=(document.getElementById("place-search").value||"").toLowerCase();grid.innerHTML="";cities.forEach(x=>{const okKind=active==="all"||active===x[2]||(active==="story"&&x[4].length);const hay=(display(x)+" "+x[0]+" "+x[1]+" "+x.slice(3).flat(3).join(" ")).toLowerCase();if(okKind&&(!q||hay.includes(q))){const w=document.createElement("div");w.innerHTML=card(x);const el=w.firstChild;el.addEventListener("click",()=>open(x));el.addEventListener("keydown",e=>{if(e.key==="Enter"||e.key===" "){e.preventDefault();open(x)}});grid.append(el)}})}
async function encounters(x){if(!encDoc){const url=fr?"Localisations.html":"Locations.html";try{const html=await fetch(url).then(r=>r.text());encDoc=new DOMParser().parseFromString(html,"text/html")}catch(e){return ""}}const aliases=[x[0].toLowerCase(),x[1].toLowerCase()];const rows=[...encDoc.querySelectorAll("#loc tbody tr")].filter(r=>aliases.includes((r.children[0]?.textContent||"").trim().toLowerCase()));if(!rows.length)return {hasRows:false,html:'<p class="muted">'+(fr?"Aucune rencontre sauvage dans cette zone.":"No wild encounters in this area.")+'</p>'};return {hasRows:true,html:'<div class="route-encounters">'+rows.map(r=>'<div><strong>'+r.children[1].textContent+'</strong><span>'+r.children[2].textContent+' · '+r.children[3].textContent+'</span><small>'+r.children[4].textContent+'</small></div>').join("")+'</div>'}}
async function open(x){content.innerHTML='<span class="eyebrow">'+(x[2]==="city"?(fr?"VILLE":"CITY"):(fr?"ROUTE":"ROUTE"))+'</span><h1>'+display(x)+'</h1>'+(fr&&x[0]!==x[1]?'<p class="muted">Nom interne/EN : '+x[0]+'</p>':'')+section(names.story,x[4])+section(names.interest,x[3])+section(names.loot,x[5])+section(names.special,x[6])+'<section><h2>'+names.enc+'</h2><div id="route-enc-box"><p class="muted">…</p></div><span id="route-enc-link"></span></section>';if(!dialog.open)dialog.showModal();const box=document.getElementById("route-enc-box");const result=await encounters(x);box.innerHTML=result.html;if(result.hasRows)document.getElementById("route-enc-link").innerHTML='<a class="detail-location-link" href="'+(fr?"Localisations.html?q=":"Locations.html?q=")+encodeURIComponent(display(x))+'">'+names.see+' →</a>'}
function section(title,arr){return '<section><h2>'+title+'</h2>'+(arr.length?'<ul>'+arr.map(v=>'<li>'+v+'</li>').join("")+'</ul>':'<p class="muted">'+names.none+'</p>')+'</section>'}
document.getElementById("place-search").addEventListener("input",render);document.querySelectorAll(".place-tab").forEach(b=>b.addEventListener("click",()=>{document.querySelectorAll(".place-tab").forEach(x=>x.classList.remove("active"));b.classList.add("active");active=b.dataset.kind;render()}));document.querySelector(".place-close").addEventListener("click",()=>dialog.close());dialog.addEventListener("click",e=>{if(e.target===dialog)dialog.close()});render();
})();