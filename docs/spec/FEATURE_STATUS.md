# Suivi des features — Pokémon Tactica V1

Ce registre décrit l’état fonctionnel courant. Il ne sert pas d’historique de décisions : les anciens états sont dans Git.

## Tags

| Tag | Sens |
|---|---|
| `[DONE]` | Développement et validation automatisée pertinents terminés. Un contrôle ROM de confort peut rester listé sans rouvrir la feature. |
| `[PARTIEL]` | Une partie importante est intégrée mais un contrôle runtime/ROM ou un sous-contrat reste ouvert. |
| `[À CORRIGER]` | Écart concret connu entre l’état actuel et le contrat. |
| `[À FAIRE]` | Fonction V1 requise non commencée. |
| `[À TESTER]` | Implémenté, mais la décision de correction dépend d’un premier vrai playtest sur la candidate courante. |
| `[HORS V1]` | Reporté après V1. |

## État courant

| Feature | Statut | Preuve / contrôle restant |
|---|---|---|
| Vitesse native x1–x4 | `[DONE]` | Tests natifs existants ; ne pas réécrire sans défaut démontré. |
| Audio indépendant / preset | `[DONE]` | Tests audio existants ; contrôle auditif final uniquement. |
| Shiny Rate | `[DONE]` | Menu et persistance couverts. |
| Difficulty / HARD | `[PARTIEL]` | Runtime et écran de configuration intégrés ; visibilité et persistance restent à revalider dans une ROM fraîche avant toute nouvelle correction. |
| Shiny Only | `[HORS V1]` | À reprendre après stabilisation V1. |
| 30 starters + Évoli | `[DONE]` | Logique cœur intégrée. |
| Sélecteur — curseur initial / annulation | `[PARTIEL]` | Correction intégrée automatiquement ; comportement visuel à revalider en ROM fraîche. |
| Œuf d’Orme / second starter distinct | `[DONE]` | Garde-fou intégré et flow déjà validé ; PR #48 ajoute le message dédié en cas de doublon sans réécrire le flow. Relecture visuelle du texte sur la prochaine candidate uniquement. |
| Évolutions — conformité 9G / Fil Liaison | `[DONE]` | Runtime migré, mécaniques impossibles traitées, validateur générique intégré à la CI et assets d’évolution du Pokédex régénérés depuis les sources runtime. |
| Rival — starter fixe / progression réelle | `[PARTIEL]` | Les 7 combats sont synchronisés depuis `rival.json` sur le prochain cap majeur : 17, 29–32, 35–38, 61–64, 65–67, 95 et 95. Tailles réelles 1→4→6, Méga dès le combat 4 ; observation ROM encore utile. |
| Rocket — phases scénario / Méga FINAL | `[PARTIEL]` | Les sept combats réels sont protégés : Proton 3→6, Petrel/Ariana 4→6, Archer 6 ; niveaux dynamiques dernier jalon +2 et Méga FINAL. Combats ROM à observer. |
| Archer = Méga-Sharpedo immédiat | `[PARTIEL]` | Set et garde-fou automatisés ; transformation à observer en ROM. |
| Mega Ring après Mortimer | `[PARTIEL]` | Ordre scripté et validé ; réception/utilisation à confirmer en ROM. |
| Méga uniques globalement | `[PARTIEL]` | Unicité automatisée ; transformations boss à observer en ROM. |
| Une Méga par Champion dès Mortimer | `[PARTIEL]` | Validateur exige exactement une Méga pour chaque Champion concerné. |
| Progression Kanto +5 | `[DONE]` | Major Bob 75 ; groupe Morgane/Erika/Jeannine 80 ; Ondine 85 ; Pierre 90 ; Auguste 95 ; Blue et deuxième Ligue 100. Test natif dédié. |
| Match retour du Maître | `[PARTIEL]` | Nouvelle équipe canonique/runtime niveau 100 et objets uniques ; combat à observer en ROM. |
| Jeannine : Aéromite ace + Méga-Kravarech | `[PARTIEL]` | Aéromite reste l’ace ; Méga-Kravarech remplace Gaulet au slot 3. Canonique/runtime/Dragalgite/Adaptabilité/validateur intégrés ; combat non observé en ROM. |
| Soins des Champions | `[DONE]` | Au plus deux soins selon la progression ; ne pas recoder sans défaut concret. |
| Encounters 4 slots 30/30/30/10 | `[DONE]` | 405 tables standard + pools spéciaux structurés. |
| Premier accès réel / scaling | `[DONE]` | Déblocage des méthodes séparé du niveau de la zone. |
| Route 36 14–17 | `[DONE]` | Source/runtime synchronisés ; témoin ROM conservé pour candidate. |
| Scorplane après badge 2 | `[DONE]` | Route 34 jour, niveaux 25–28, slot 30 % à la place de Migalos ; aucune famille Scorplane avant le cap badge 2. |
| Wattouat disponible | `[DONE]` | Route 31 jour, slot 30 %. |
| Couverture globale pré-Ligue | `[DONE]` | Dataset standard : 479 espèces utilisées et 479 disponibles avant/à la Ligue ; 0 espèce utilisée reste uniquement post-Ligue. 72 slots ont été remplacés sur 43 tables ; 125 espèces restent dupliquées avant Ligue, ce qui est acceptable puisque la couverture est assurée. |
| Headbutt | `[DONE]` | 4 tables dédiées. |
| Safari | `[DONE]` | 53 pools. |
| Wiki joueur FR/EN | `[DONE]` | Site HTML : Accueil remake 2G, Guide illustré, Kanto développé, Routes/Villes, navigation FR/EN et fiches Pokédex détaillées intégrés. Les sources Markdown sont maintenues en miroir ; ne pas refaire le style global. |
| Pokédex — méthodes d’évolution | `[DONE]` | Les champs `evolutions` de `tactica-species-*.js` sont générés depuis le runtime par `sync_tactica_species_evolutions.py` et protégés par `--check` en CI. |
| Sync Localisations — niveaux | `[DONE]` | Générateur canonique. |
| Sync Localisations — espèces / recherche | `[DONE]` | Générateur étendu aux espèces et `data-search`; synchronisations `--check` et CI Tactica vertes sur le HEAD de la PR de nettoyage. |
| Compteurs Pokédex | `[DONE]` | Recalculés depuis les tables standard + pools spéciaux visibles ; synchronisation idempotente confirmée par la CI Tactica. |
| Summary ROM | `[PARTIEL]` | Interface inspirée de Pokémon Épée/Bouclier intégrée, avec pages Summary usuelles et page IV/EV. Compilation et validation statique vertes ; contrôle ROM requis. |
| HUD combat | `[PARTIEL]` | Design Noir/Blanc d’origine conservé. La Méga reste interactive sur `START` et l’aide des capacités utilise `R`. Le fond de texte opaque qui masquait les environnements est retiré et la réserve mémoire couvre la reconstruction du décor après un passage par l’équipe. Compilation et validations ciblées vertes ; combat et changement de Pokémon à recontrôler en ROM. |
| Menu principal / Pokédex / sac / carte Dresseur | `[PARTIEL]` | Sac Épée/Bouclier et Pokédex HGSS intégrés. En jeu normal, le menu Start plein écran reprend une présentation HGSS/BW ; les contextes spéciaux gardent le menu compact fiable. La liste des fenêtres du menu Start est bornée, son chargement est étalé sur plusieurs images et ses fonds statiques sont envoyés directement en VRAM afin d’éviter la corruption des polices et de réduire la pression mémoire. La carte Dresseur utilise un habillage Épée/Bouclier neutralisé pour laisser le moteur afficher les textes localisés et conserve les 16 badges. Compilation et validation statique vertes ; contrôle ROM requis. |
| UI boutiques | `[PARTIEL]` | La boutique animée est désactivée. Le moteur natif stable utilise un habillage statique B2W2 « 2020s Edition », sans boucle d’animation ni allocation additionnelle. Compilation et validation statique vertes ; parcours d’achat ROM à recontrôler. |
| Assistant d’entraînement | `[DONE]` | Tests dédiés existants. |
| Boutiques objets/moves/Méga-Gemmes — contenu | `[PARTIEL]` | Catalogues/runtime présents ; parcours d’achat ROM à revalider. |
| Balance 26 espèces / 16 learnsets | `[DONE]` | Source canonique + validateur dédié ; Draco-Griffe est légal pour Hydragon au match retour. |
| Intro Chen / HARD / Recommended | `[DONE]` | PR #48 : speech vanilla remplacé par l’intro Tactica, avertissements HARD/Recommended/Custom clarifiés. |
| IA HARD | `[À TESTER]` | Aucun correctif à faire avant le premier vrai playtest HARD de la version courante. Les anciens cas Mimiqui/Cornèbre/Téraclope sont seulement des témoins à surveiller. |
| Écran titre — centrage « Pokémon Tactica » | `[DONE]` | Le libellé `TACTICA` est centré dans sa bulle sans déplacer l'ensemble de la bannière ; le validateur UI protège le centrage interne et la position écran. |

## Règle de merge

Une feature automatisée propre n’attend pas le playtest owner final pour rejoindre `integration/v1`. Elle doit cependant conserver dans ce fichier ou `PLAYTEST_STATUS.md` les observations ROM encore utiles.

Ne jamais transformer « non testé manuellement » en « non implémenté », et ne jamais déclarer un contrôle ROM effectué s’il ne l’a pas été.

## Prochain ordre de travail

1. reconstruire une ROM candidate propre depuis le SHA final ;
2. effectuer le playthrough owner complet ; l’IA HARD est à tester avant toute correction.
