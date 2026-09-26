# Boutiques et assistant

## Charte UI des boutiques custom

Les menus custom Tactica de boutique suivent la charte ROM : **fond rouge, texte noir, séparateurs sombres**, sélection clairement visible sans grand surlignage blanc. Les menus génériques HnS ne sont pas considérés conformes simplement parce que le catalogue fonctionne.

Contrôles ROM obligatoires : catégories, sous-catégories, retour arrière, texte long, liste vide, fonds insuffisants et retour au menu précédent.

## PNJ capacités / CT

Rendre achetables **toutes les capacités utilisables dans le build**, notamment Boutefeu et Colère, même si elles n'ont pas d'objet CT natif. Une interface de catalogue de Move IDs est acceptable : prix, catégories, compatibilité du Pokémon, confirmation et apprentissage. L'achat n'autorise pas un Pokémon à apprendre hors de son movepool.

Le catalogue Tactica est généré depuis `all_learnables.json` et couvre 824 Move IDs jouables présents dans les movepools de 1 110 entrées d'espèces/formes. Il exclut les Z-Moves, Max Moves, placeholders et Struggle, puis filtre la liste affichée selon le Pokémon choisi et ses capacités déjà connues. Boutefeu et Colère sont explicitement couvertes. Une leçon confirmée coûte 3000 Pokédollars ; l'argent n'est retiré qu'après apprentissage effectif.

## PNJ objets

Proposer les objets utiles qui ne sont pas déjà vendus par les marchands habituels : évolution, objets tenus et de combat, baies et préparation stratégique. Exclure le stock normal de soins, statut, Balls, Repousse, Corde Sortie, Key Items, objets de quête/progression et incohérences scénaristiques.

L'inventaire audité contient 259 objets spécialisés uniques. Les Cristaux Z, Méga-Gemmes et objets de progression restent hors de ce catalogue.

## Méga-Gemmes

Un PNJ Méga-Gemmes dédié est placé près du PNJ capacités/CT. Chaque Méga-Gemme vendue coûte exactement **3000 Pokédollars**. Tester catalogue, emplacement, achat, retrait de l'argent et réception de l'objet dans la ROM.

Le vendeur de gemmes ne remplace pas le déblocage de la mécanique : le **Mega Ring** est remis par Mortimer après le badge 4.

## Entraînement

L'assistant centralisé permet EXP jusqu'au cap, IV, EV et remise à zéro, nature, talent et bonheur. Le prix/les restrictions ne doivent pas annuler l'objectif de stratégie sans grind.
