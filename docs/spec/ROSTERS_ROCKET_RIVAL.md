# Équipes progressives : rival et Team Rocket

## Statut de cette spécification

Décision owner validée le 26-09-2026.

Cette page remplace les anciennes compositions rival/Rocket lorsqu'elles sont en contradiction avec elle. Les fichiers `data/spec/rival.json`, `data/spec/rocket_progression.json` et les entrées Rocket de `data/spec/bosses.json` doivent être régénérés/alignés par Codex à partir de cette spécification avant de pouvoir de nouveau être considérés canoniques.

Règles communes :

- combats solo 1v1 ;
- équipes construites comme de vraies équipes Singles, pas comme des listes thématiques ;
- aucun légendaire/fabuleux ;
- maximum **2 familles de starters Tactica par équipe** ;
- objets tenus uniques dans une même équipe ;
- toute équipe climat/terrain possède **2 setters** ; priorité aux setters par talent ;
- si aucun second setter par talent non-légendaire n'existe dans le build, un setter manuel est accepté ;
- EARLY/MID conservent l'identité stratégique de FINAL avec moves/stades légaux au niveau réel ;
- la Méga du rival n'est utilisable qu'après obtention du badge 4 par le joueur ;
- les Méga Rocket n'apparaissent que dans leur équipe FINAL ;
- toute Méga doit avoir une forme de base avec talent légal avant transformation ;
- l'IA ne doit pas utiliser Tour Rapide s'il n'y a aucun hazard côté utilisateur du move.

---

# Rival

## Sélection et progression

Le rival ne tire plus un starter aléatoire parmi cinq espèces d'une catégorie. Chaque archétype possède désormais un **starter fixe**.

Correspondances de catégorie :

| Catégorie joueur | Archétype rival possible |
|---|---|
| Feu | Eau ou Sol |
| Eau | Plante ou Électrik |
| Plante | Feu ou Glace |
| Électrik | Sol ou Plante |
| Sol | Eau ou Glace |
| Glace | Feu ou Eau |

Une catégorie rival est tirée une fois parmi les choix autorisés puis sauvegardée.

Progression des effectifs :

- premier combat : starter fixe seul, niveau 17, stade légal au niveau 17 ;
- après badge 1 : 3 Pokémon ;
- après badge 2 : 4 Pokémon ;
- après badge 3 : équipe complète de 6 ;
- après badge 4 : la même équipe complète peut utiliser sa Méga.

Les membres introduits restent présents dans les combats suivants.

---

## Rival Feu — Sun

**Starter fixe : famille Poussifeu.**

Équipe finale :

1. **Chartor** — setter #1
2. **Braségali → Méga-Braségali** — starter / ace
3. **Florizarre** — Chlorophylle
4. **Feunard** — setter #2
5. **Fragilady de Hisui** — Chlorophylle physique
6. **Fort-Ivoire** — Protosynthèse

### Sets FINAL

**Chartor @ Grosses Bottes**
- Talent : Sécheresse
- Piège de Roc
- Tour Rapide
- Ébullilave
- Bâillement

Règle IA : Tour Rapide seulement si des hazards sont présents du côté de Chartor.

**Méga-Braségali @ Braségalite**
- Talent avant Méga : talent légal de Braségali
- Talent Méga : Turbo
- Boutefeu
- Close Combat
- Rapace
- Abri

**Florizarre @ Orbe Vie**
- Talent : Chlorophylle
- Croissance
- Giga-Sangsue
- Bomb-Beurk
- Telluriforce

**Feunard @ Roche Chaude**
- Talent : Sécheresse
- Machination
- Lance-Flammes
- Lance-Soleil
- Vibrobscur

Déflagration n'est pas retenue : elle reste à 85 % de précision sous le Soleil.

**Fragilady de Hisui @ Ceinture Pro**
- Talent : Chlorophylle
- Danse Victoire
- Lame Feuille
- Close Combat
- couverture physique légale à valider dans le build

**Fort-Ivoire @ Veste de Combat**
- Talent : Protosynthèse
- Headlong Rush
- Close Combat
- Sabotage
- Tête de Fer

Familles starters utilisées : Poussifeu + Bulbizarre = 2.

---

## Rival Eau — Rain

**Starter fixe : famille Gobou.**

Équipe finale :

1. **Bekipan** — setter #1
2. **Laggron → Méga-Laggron** — starter / ace
3. **Hyporoi** — sweeper spécial Glissade
4. **Tarpaud** — setter #2
5. **Hastacuda** — breaker Glissade
6. **Hydragon** — revenge killer Prognathe

### Sets FINAL

**Bekipan @ Roche Humide**
- Talent : Crachin
- Vent Violent
- Surf
- Demi-Tour
- Atterrissage

**Méga-Laggron @ Laggronite**
- Talent avant Méga : talent légal de Laggron
- Talent Méga : Glissade
- Cascade
- Séisme
- Poing Glace
- Flip Turn / move pivot légal équivalent

**Hyporoi @ Orbe Vie**
- Talent : Glissade
- Surf
- Draco-Choc
- Laser Glace
- Vent Violent

**Tarpaud @ Restes**
- Talent : Crachin
- Ébullition
- Encore
- Requiem
- Laser Glace

**Hastacuda @ Bandeau Choix**
- Talent : Glissade
- Aqua-Brèche / Liquidation selon disponibilité
- Close Combat
- Psycho-Croc
- Flip Turn

**Hydragon @ Mouchoir Choix**
- Talent : Prognathe
- Branchicrok
- Mâchouille
- Psycho-Croc
- Crocs Givre

Familles starters utilisées : Gobou + Hypotrempe = 2.

---

## Rival Plante — Grassy Terrain

**Starter fixe : famille Arcko.**

Équipe finale :

1. **Gorythmic** — setter #1
2. **Jungko → Méga-Jungko** — starter / ace
3. **Brutalibré** — Graine Herbe + Délestage
4. **Noacier** — hazards / sustain
5. **Gromago** — anti-removal / breaker spécial
6. **Arboliva** — setter #2 via Graine Semence

### Sets FINAL

**Gorythmic @ Champ'Duit**
- Talent : Créa-Herbe
- Gliss'Herbe
- Martobois
- Sabotage
- Demi-Tour

**Méga-Jungko @ Jungkite**
- profil Tactica physique
- Danse Draco
- Lame Feuille
- Draco-Griffe
- Éboulement

**Brutalibré @ Graine Herbe**
- Talent : Délestage
- Danse-Lames
- Acrobatie
- Close Combat
- Encore

**Noacier @ Restes**
- Talent : Épine de Fer
- Piège de Roc
- Vampigraine
- Mégafouet
- Gyroballe

Mégafouet remplace Picots : le Terrain Herbu renforce le STAB Plante et évite de surcharger le plan hazards.

**Gromago @ Ballon**
- Talent : Corps en Or
- Machination
- Ruée d'Or
- Ball'Ombre
- Soin

**Arboliva @ Baie Sitrus**
- Talent : Graine Semence
- Giga-Sangsue
- Mégaphone
- Telluriforce
- Vole-Force

Familles starters utilisées : Arcko + Ouistempo = 2.

---

## Rival Électrik — Electric Terrain

**Starter fixe : famille Élekid.**

Équipe finale :

1. **Wattapik** — setter automatique
2. **Élekable** — starter / breaker
3. **Raichu d'Alola** — Surf Caudal
4. **Salarsen** — setter manuel de secours
5. **Paume-de-Fer** — Charge Quantique
6. **Élecsprint → Méga-Élecsprint** — Méga / pivot

Note moteur : seuls Wattapik et Tokorico possèdent Créa-Élec dans le build actuel. Tokorico étant exclu avec les légendaires/fabuleux, le second setter est volontairement manuel.

### Sets FINAL

**Wattapik @ Champ'Duit**
- Talent : Créa-Élec
- Picots
- Soin
- Décharge
- Souvenir

**Élekable @ Ceinture Pro**
- Talent Tactica : Motorisé
- STAB Électrik physique légal du build
- Séisme
- Poing Glace
- Poing Feu

**Raichu d'Alola @ Orbe Vie**
- Talent : Surf Caudal
- Machination
- Tonnerre
- Psyko
- Surf

**Salarsen @ Spray Gorge**
- Talent : Punk Rock
- Champ Électrifié
- Overdrive
- Bang Sonique
- Bomb-Beurk

**Paume-de-Fer @ Veste de Combat**
- Talent : Charge Quantique
- Vampi-Poing
- Éclair Fou
- Poing Glace
- Change Éclair / couverture légale équivalente

**Méga-Élecsprint @ Élecsprintite**
- Talent pré-Méga légal
- Talent Méga : Intimidation
- Tonnerre
- Surchauffe
- Change Éclair
- Rayon Signal / couverture spéciale légale équivalente

Familles starters utilisées : Élekid uniquement.

---

## Rival Sol — Sandstorm

**Starter fixe : famille Rototaupe.**

Équipe finale :

1. **Hippodocus** — setter #1
2. **Minotaupe** — starter / Brise Moule
3. **Tyranocif → Méga-Tyranocif** — setter #2 / Méga
4. **Carchacrok** — sweeper
5. **Scorvol** — Soin Poison
6. **Corvaillus** — mur / pivot

### Sets FINAL

**Hippodocus @ Roche Lisse**
- Talent : Sable Volant
- Piège de Roc
- Séisme
- Paresse
- Cyclone

**Minotaupe @ Orbe Vie**
- Talent : Brise Moule
- Danse-Lames
- Séisme
- Tête de Fer
- Éboulement

**Méga-Tyranocif @ Tyranocivite**
- Talent pré-Méga : Sable Volant
- Talent Méga : Sable Volant
- Danse Draco
- Mâchouille
- Éboulement
- Poing Glace

**Carchacrok @ Baie Nanone**
- Talent : Voile Sable
- Danse-Lames
- Séisme
- Draco-Griffe
- Tête de Fer

**Scorvol @ Orbe Toxique**
- Talent : Soin Poison
- Séisme
- Sabotage
- Toxic
- Atterrissage

**Corvaillus @ Restes**
- Talent : Armure Miroir
- Mur de Fer
- Big Splash
- Rapace
- Atterrissage

Familles starters utilisées : Rototaupe uniquement.

---

## Rival Glace — Snow

**Starter fixe : famille Darumarond de Galar.**

Équipe finale :

1. **Sorbouboul** — setter #1
2. **Darumacho de Galar** — starter / breaker
3. **Dragmara** — setter #2
4. **Sablaireau d'Alola** — Chasse-Neige
5. **Hydragla** — Voile Aurore / Chasse-Neige
6. **Glaivodo → Méga-Glaivodo** — ace

### Sets FINAL

**Sorbouboul @ Roche Glace**
- Talent : Alerte Neige
- Blizzard
- Lyophilisation
- couverture spéciale non-Glace légale
- Provoc

Ne pas utiliser Ball'Météo ici : Blizzard + Lyophilisation suffisent déjà comme STAB Glace.

**Darumacho de Galar @ Bandeau Choix**
- talent légal du build
- Chute Glace
- Boutefeu
- Séisme
- Demi-Tour

**Dragmara @ Lunettes Sages**
- Talent : Alerte Neige
- Blizzard
- Lyophilisation
- Telluriforce
- Tonnerre **si réellement teachable dans le build**, sinon couverture Électrik légale équivalente

**Sablaireau d'Alola @ Orbe Vie**
- Talent : Chasse-Neige
- Danse-Lames
- Chute Glace
- Tête de Fer
- Séisme

**Hydragla @ Lumargile**
- Talent : Chasse-Neige
- Chute Glace
- Voile Aurore
- Branchicrok
- Éboulement

**Méga-Glaivodo @ Glaivodite**
- talent pré-Méga légal
- Danse Draco
- Glaive Rush
- Chute Glace
- Séisme

Familles starters utilisées : Darumarond-G + Sorbébé = 2.

---

# Team Rocket

Les Exécutifs restent sur une progression 3 -> 4 -> 6. Les familles introduites persistent dans les versions suivantes.

Les thèmes Rocket n'imposent pas deux setters météo/terrain lorsqu'ils ne reposent pas sur une météo/terrain.

Les Méga n'apparaissent que dans la version FINAL.

---

## Proton — Poison offense / hazards

### FINAL

1. **Floréclat @ Ceinture Force**
   - Talent : Dépôt Toxique
   - Piège de Roc
   - Toupie Éclat
   - Rayon Gemme
   - Telluriforce

2. **Nostenfer @ Grosses Bottes**
   - Talent : Infiltration
   - Rapace
   - Direct Toxik
   - Demi-Tour
   - Atterrissage

3. **Brutapode @ Orbe Vie**
   - Talent : Turbo
   - Mégacorne
   - Direct Toxik
   - Séisme
   - Abri

4. **Coatox @ Ceinture Pro**
   - Talent : Peau Sèche
   - Close Combat
   - Détricanon
   - Coup Bas
   - Poing Glace

5. **Grotadmorv d'Alola @ Veste de Combat**
   - Talent : Toxitouche
   - Détricanon
   - Sabotage
   - Poing Feu
   - Ombre Portée

6. **Dardargnan -> Méga-Dardargnan @ Dardargnite**
   - talent pré-Méga légal
   - Talent Méga : Adaptabilité
   - Demi-Tour
   - Direct Toxik
   - Tunnelier
   - Sabotage

Ace FINAL : Méga-Dardargnan.

Progression de familles :
- EARLY 3 : Nosferapti / Venipatte / Tadmorv-A
- MID 4 : + Cradopaud
- FINAL 6 : + Floréclat / Dardargnan

---

## Petrel — illusion / disruption / sacrifice

### FINAL

1. **Métamorph @ Mouchoir Choix**
   - Talent : Imposteur
   - Morphing

2. **Zoroark de Hisui @ Orbe Vie**
   - Talent : Illusion
   - Mégaphone
   - Ball'Ombre
   - Lance-Flammes
   - Demi-Tour

3. **Smogogo @ Boue Noire**
   - Talent : Lévitation
   - Feu Follet
   - Provoc
   - Balance / partage de PV légal disponible
   - Explosion

4. **Électrode @ Ceinture Force**
   - Talent : Boom Final
   - Tonnerre
   - Provoc
   - Cage Éclair
   - Explosion

5. **Angoliath @ Lumargile**
   - Talent : Farceur
   - Protection
   - Mur Lumière
   - Dernier Mot
   - Choc Émotionnel

6. **Branette -> Méga-Branette @ Branettite**
   - talent pré-Méga légal
   - Talent Méga : Farceur
   - Feu Follet
   - Sabotage
   - Prélèvement Destin
   - Ombre Portée

Ace FINAL : Méga-Branette.

Progression de familles :
- EARLY 3 : Métamorph / Smogo / Zorua-H
- MID 4 : + Voltorbe
- FINAL 6 : + Grimalin / Branette

---

## Ariana — status / contrôle / snowball

### FINAL

1. **Arbok @ Boue Noire**
   - Talent : Intimidation
   - Regard Médusant
   - Détricanon
   - Séisme
   - Mâchouille

2. **Roserade @ Ceinture Force**
   - Talent : Médic Nature
   - Poudre Dodo
   - Pics Toxik
   - Giga-Sangsue
   - Bomb-Beurk

3. **Malamandre @ Grosses Bottes**
   - Talent : Corrosion
   - Toxic
   - Machination
   - Lance-Flammes
   - Cradovague

4. **Nidoqueen @ Orbe Vie**
   - Talent : Sans Limite
   - Telluriforce
   - Laser Glace
   - Tonnerre
   - Bomb-Beurk

5. **Corboss @ Bec Pointu**
   - Talent : Impudence
   - Rapace
   - Coup Bas
   - Surpuissance
   - Atterrissage

6. **Absol -> Méga-Absol @ Absolite**
   - talent pré-Méga légal
   - Talent Méga : Miroir Magik
   - Danse-Lames
   - Sabotage
   - Coup Bas
   - Câlinerie

Ace FINAL : Méga-Absol.

Progression de familles :
- EARLY 3 : Abo / Rozbouton-Rosélia / Cornèbre
- MID 4 : + Tritox
- FINAL 6 : + Nidoran♀ / Absol

---

## Archer — Dark offense / boss final Rocket

### FINAL

1. **Nostenfer @ Grosses Bottes**
   - Talent : Infiltration
   - Rapace
   - Direct Toxik
   - Demi-Tour
   - Atterrissage

2. **Dimoret @ Bandeau Choix**
   - Talent légal du build
   - Chute Glace
   - Sabotage
   - Éclats Glace
   - Balayage

3. **Nidoking @ Orbe Vie**
   - Talent : Sans Limite
   - Telluriforce
   - Cradovague
   - Laser Glace
   - Tonnerre

4. **Magnézone @ Veste de Combat**
   - Talent : Analyste
   - Tonnerre
   - Luminocanon
   - Change Éclair
   - Rayon Signal

5. **Trousselin @ Lumargile**
   - Talent : Farceur
   - Protection
   - Mur Lumière
   - Zénith
   - Cage Éclair

6. **Démolosse -> Méga-Démolosse @ Démolossite**
   - talent pré-Méga légal
   - Talent Méga : Force Soleil
   - Machination
   - Vibrobscur
   - Lance-Flammes
   - Bomb-Beurk

Zénith de Trousselin sert explicitement à activer Force Soleil sans transformer Archer en équipe Sun complète.

Ace FINAL : Méga-Démolosse.

Progression de familles :
- EARLY 3 : Farfuret / Nosferapti / Malosse
- MID 4 : + Nidoran♂
- FINAL 6 : + Magnéti / Trousselin

---

# Obligations d'implémentation

Codex doit, dans cet ordre :

1. remplacer les anciennes compositions dans `data/spec/rival.json`, `data/spec/rocket_progression.json` et `data/spec/bosses.json` ;
2. adapter la sélection rival pour utiliser un **starter fixe par archétype** et non plus un tirage parmi cinq starters ;
3. conserver le tirage aléatoire de l'archétype rival parmi les catégories autorisées par la catégorie joueur ;
4. dériver EARLY/MID depuis les familles FINAL en respectant niveaux, évolutions, moves et objets disponibles ;
5. activer la Méga rival uniquement après badge 4 ;
6. activer les Méga Rocket uniquement dans FINAL ;
7. valider automatiquement maximum 2 familles starters par équipe rival ;
8. valider les deux setters pour Sun/Rain/Grassy/Sand/Snow et l'exception Electric : Wattapik automatique + Salarsen manuel ;
9. valider unicité des objets ;
10. valider la légalité de chaque move au niveau réel ;
11. vérifier spécifiquement Tonnerre sur Dragmara ; si non teachable, choisir une couverture Électrik légale et documenter le remplacement ;
12. ne pas modifier les événements scénario : les nouveaux rosters s'appliquent aux combats existants.
