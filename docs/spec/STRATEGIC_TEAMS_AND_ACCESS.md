# Règles stratégiques des équipes Tactica

## Unicité des objets tenus

Dans toute équipe d'un dresseur fixe important, un même objet tenu ne peut apparaître qu'une seule fois.

Cette règle concerne notamment :

- Champions ;
- Conseil 4 ;
- Maître ;
- rival ;
- Exécutifs Rocket ;
- autres boss fixes.

Exemples interdits :

- deux Évoluroc dans la même équipe ;
- deux Restes ;
- deux Orbe Vie ;
- deux Lunettes Choix ;
- deux Mouchoir Choix ;
- deux Ceinture Force ;
- plusieurs exemplaires de n'importe quel autre objet tenu identique.

Cette règle concerne les objets tenus par les Pokémon. Les objets de soin utilisés directement par le dresseur pendant le combat suivent leur propre règle.

Lorsqu'un objet est demandé par plusieurs membres de l'équipe, l'ordre de priorité est :

1. objet indispensable au fonctionnement du set ou de la stratégie ;
2. objet permettant l'activation d'une mécanique essentielle ;
3. objet fortement synergique avec talent/moveset ;
4. simple amplification de dégâts ou de statistiques.

Le Pokémon dont le fonctionnement dépend le plus de l'objet conserve celui-ci.

L'autre Pokémon reçoit un objet différent cohérent ou aucun objet.

Un validateur automatique doit rejeter toute équipe canonique contenant deux fois le même objet tenu non nul.

---

# Limitation des objets — premières arènes

Afin de conserver une difficulté progressive :

## Albert

Maximum :

**1 Pokémon équipé**

Objet retenu :

- Cornèbre : Ceinture Force

Scorplane et Roucoups n'ont pas d'objet tenu.

## Hector

Maximum :

**2 Pokémon équipés**

Objets retenus :

- Pomdepik : Baie Sitrus
- Insécateur : Évoluroc

Yanma et Scarhino n'ont pas d'objet tenu.

## Blanche

Maximum :

**3 Pokémon équipés**

Objets retenus :

- Porygon2 : Évoluroc
- Ursaring : Orbe Flamme
- Famignol : Loupe

Évoli, Capidextre et Écrémeuh n'ont pas d'objet tenu.

À partir de Mortimer, l'équipe complète peut être équipée, sous réserve de respecter l'unicité des objets.

---

# Progression des movesets

Les premiers combats ne doivent pas utiliser directement les sets compétitifs finaux.

Trois niveaux de sets existent :

## EARLY

Utilisé en début de progression.

Caractéristiques :

- attaques de puissance modérée ;
- couverture limitée ;
- peu d'objets ;
- mécanique principale du dresseur déjà visible ;
- pas de combinaison endgame disproportionnée.

## MID

Utilisé lorsque la progression devient plus avancée.

Caractéristiques :

- meilleurs STAB ;
- accès progressif aux attaques de couverture ;
- setup plus développé ;
- davantage d'objets ;
- stratégie plus complète.

## FINAL

Set canonique complet prévu pour la fin de progression du dresseur.

Les versions EARLY et MID doivent conserver l'identité stratégique de la version finale sans simplement copier toutes ses attaques et tous ses objets.

Exemple interdit :

Scarhino niveau 22 avec immédiatement :

Close Combat / Megahorn / Knock Off / Facade

Le combat doit rester difficile mais raisonnablement gagnable avec les ressources disponibles à ce stade.

---

# Corrections d'objets sur les équipes finales

## Jasmine

- Corvaillus : Restes
- Pondralugon : Baie Sitrus

Ne pas utiliser deux Restes.

## Clément

- Xatu : Orbe Vie
- Gallame : Ceinture Pro

## Marion

- Corboss : Orbe Vie
- Dimoret : Ceinture Pro

## Petrel

- Grotadmorv : Boue Noire
- Smogogo : Baie Sitrus

## Ariana

- Malamandre : Ceinture Force
- Gribouraigne : Baie Sitrus
- Nidoqueen : Orbe Vie
- Corboss : Bec Pointu

## Archer

- Nidoking : Orbe Vie
- Démolosse : Ceinture Force
- Dimoret : Ceinture Pro

## Pierre

- Gigansel : Restes
- Vacilys : Baie Sitrus

## Jeannine

- Prédastérie : Boue Noire
- Smogogo de Galar : Baie Sitrus
- Aéromite : Ceinture Force

## Auguste

- Chartor : Roche Chaude
- Feunard : Restes

Toutes les autres équipes canoniques doivent également être validées automatiquement contre la règle d'unicité.

---

# Team Rocket

Chaque Exécutif possède trois versions de roster définies à l'avance :

- équipe de 3 ;
- équipe de 4 ;
- équipe finale de 6.

Les Pokémon déjà introduits restent présents dans les versions suivantes.

Les membres et sets précis sont décrits dans [ROSTERS_ROCKET_RIVAL.md](ROSTERS_ROCKET_RIVAL.md) et dans `data/spec/rocket_progression.json` (EARLY/MID) et `data/spec/bosses.json` (FINAL).

Le stade d'évolution réel est déterminé par le niveau du combat.

## Proton

### 3 Pokémon
- famille Nosferapti
- famille Rattata
- famille Tadmorv d'Alola

### 4 Pokémon
- les trois précédents
- famille Smogo

### 6 Pokémon
- les quatre précédents
- famille Venipatte
- famille Cradopaud

Équipe finale :

Nostenfer / Smogogo / Rattatac / Brutapode / Coatox / Grotadmorv d'Alola

## Petrel

### 3 Pokémon
- Métamorph
- famille Smogo
- famille Rattata

### 4 Pokémon
- les trois précédents
- famille Voltorbe

### 6 Pokémon
- les quatre précédents
- famille Tadmorv
- famille Zorua de Hisui

Équipe finale :

Métamorph / Smogogo / Électrode / Grotadmorv / Zoroark de Hisui / Rattatac

## Ariana

### 3 Pokémon
- famille Abo
- famille Mystherbe
- famille Cornèbre

### 4 Pokémon
- les trois précédents
- famille Gribouraigne

### 6 Pokémon
- les quatre précédents
- famille Nidoran♀
- famille Tritox

Équipe finale :

Arbok / Rafflesia / Tag-Tag / Nidoqueen / Malamandre / Corboss

## Archer

### 3 Pokémon
- famille Farfuret
- famille Nosferapti
- famille Malosse

### 4 Pokémon
- les trois précédents
- famille Nidoran♂

### 6 Pokémon
- les quatre précédents
- famille Magnéti
- famille Rapion

Équipe finale :

Dimoret / Nostenfer / Nidoking / Magnézone / Drascore / Démolosse

Chaque version EARLY / MID / FINAL respecte également l'unicité des objets tenus.

---

# Rival

Le starter du rival est déterminé une seule fois et reste fixe.

Progression :

- après badge 1 : 3 Pokémon ;
- après badge 2 : 4 Pokémon ;
- après badge 3 : équipe finale de 6.

Les membres déjà choisis persistent dans les combats suivants.

Le stade d'évolution dépend du niveau réel.

Les sets suivent les phases EARLY / MID / FINAL.

L'unicité des objets tenus est obligatoire dans chacune des six équipes finales et dans toutes leurs versions intermédiaires.

Les six compositions et sets de travail sont dans `data/spec/rival.json` (`fight_rosters`) et [ROSTERS_ROCKET_RIVAL.md](ROSTERS_ROCKET_RIVAL.md). Leurs propositions de capacités doivent être vérifiées au niveau du combat avant intégration moteur.

---

# Encounters — niveau et accessibilité

Pour toute table standard :

**max_level <= level cap actif lors du premier accès réel à la zone**

Safari est exclu de cette règle.

Le premier accès réel doit prendre en compte :

- connexions de cartes ;
- gates ;
- obstacles ;
- événements ;
- badges ;
- CS ;
- méthodes de rencontre réellement utilisables.

L'ordre numérique des routes n'est pas une information suffisante.

Un dataset canonique `earliest_access_cap` doit associer chaque map à son premier cap accessible.

Le validateur doit échouer si :

`table.max_level > earliest_access_cap`

Le niveau sauvage ne doit jamais permettre au joueur de dépasser artificiellement le cap de progression simplement en capturant un Pokémon.

---

# Encounters — cohérence d'habitat

Les espèces doivent correspondre à l'environnement.

Exemples :

- zones Glace : priorité forte aux familles Glace ;
- zones aquatiques : Pokémon cohérents avec l'eau ;
- grottes : espèces souterraines/minérales/nocturnes adaptées ;
- forêts : espèces cohérentes avec forêt/végétation/insectes.

Un échange destiné à corriger un starter ou un niveau doit préserver autant que possible cette cohérence.

---

# Encounters — stade d'évolution

Le stade sauvage doit être cohérent avec le niveau de la zone.

Exemple :

un Roucool niveau 49 n'est pas acceptable lorsqu'une forme évoluée de sa famille est cohérente avec ce niveau.

Pour les évolutions par niveau, utiliser la forme normalement atteignable au niveau de la rencontre.

Pour les évolutions par pierre, bonheur ou condition spéciale, choisir la forme la plus cohérente avec la progression et l'habitat.

Cette règle s'applique à toutes les espèces, pas uniquement aux starters.

---

# Familles starters sauvages

Toutes les familles des starters officiels Gen 1 à Gen 9 doivent être disponibles au moins une fois en encounters standards.

Toutes les familles des starters spécifiques du sélecteur Tactica doivent également être disponibles.

La forme de base n'est pas obligatoire.

Une évolution sauvage compte comme disponibilité de la famille.

Aucune famille starter ne doit être capturable avant l'obtention du deuxième badge.

À partir du badge 2, elles deviennent progressivement disponibles.

La première apparition de la famille doit idéalement occuper le slot rare 10 %.

Audit du 25 septembre après le bloc `feature/strategic-rules` : Germignon est ajouté au Bois aux Chênes ; Feunard d'Alola et Darumacho de Galar à la Route de Glace ; Pichu est couvert par Pikachu Route 47. Le validateur contrôle les familles et les slots rares ; l'audit exhaustif des caps d'accès des cartes reste à faire.

Dans la Route de Glace, privilégier les formes cohérentes avec les niveaux élevés de la zone, notamment Feunard d'Alola et Darumacho de Galar si approprié.

Salamèche et Élekid ont été retirés de la Route 36 et placés dans des zones accessibles après badge 2 (respectivement Route 37 et Route 42). Vérifier l'accès réel de toutes les autres tables avec `earliest_access_cap`.

---

# Level cap et milestones

Le level cap représente le prochain jalon réel que le joueur est autorisé à préparer.

Un boss Rocket utilise :

`dernier cap canonique champion/rival + 2`

Ce `+2` est temporaire.

Après victoire contre le Rocket, le cap doit immédiatement être recalculé vers le prochain jalon réel.

Un boss Rocket vaincu ne peut jamais maintenir artificiellement le cap à son propre niveau.

Le cap ne décroît jamais.

Le tableau des milestones doit correspondre à l'ordre réel des combats accessibles.

Cas de régression obligatoire :

- avant Albert : cap 17 ;
- Albert battu : cap 19 avant Proton ;
- Proton 1 battu : cap 25 avant Hector ;
- Hector doit être jouable avec cap 25 ;
- le cap ne doit jamais redescendre après Hector.

Ajouter un test automatisé explicitement dédié à la transition :

`Proton 1 -> Hector`

---

# Validateurs obligatoires

Les tests de données doivent vérifier automatiquement :

1. quatre slots réels 30/30/30/10 sur les encounters standards ;
2. max_level <= earliest_access_cap ;
3. aucune famille starter accessible avant badge 2 ;
4. présence d'au moins une forme de chaque famille starter requise ;
5. cohérence automatisable des stades d'évolution ;
6. aucun doublon d'objet tenu dans une équipe de boss/rival/Rocket ;
7. Albert <= 1 objet ;
8. Hector <= 2 objets ;
9. Blanche <= 3 objets ;
10. progression correcte des caps, notamment Proton 1 -> Hector ;
11. persistance des membres des équipes intermédiaires rival/Rocket ;
12. formes évolutives légales au niveau réel du combat.
