[Accueil](Accueil.md) · **[Jouer](Jouer.md)** · [Guide](Guide-de-jeu.md) · [Changements](Changements.md) · [Pokédex](Pokedex.md) · [Localisations](Localisations.md) · [Routes et Villes](Routes-et-Villes.html) · [Boss & Conseils](Boss-et-Conseils.md) · [Crédits](Credits-et-Versions.md) · [Roadmap](Roadmap.md) · **[EN](../EN/Play.md)**

# Jouer à Pokémon Tactica

## Télécharger le jeu

Pokémon Tactica se télécharge sous la forme d’une ROM Game Boy Advance complète. La [ROM Tactica au format .gba](https://github.com/DyHyugal/pokemon-tactica/releases/download/v1-candidate-63dbd3d9/pokemon-tactica.gba) peut être ouverte directement dans un émulateur : aucune ROM Pokémon Émeraude, version EmeraldPlus ou application de patch n’est nécessaire.

Le [pack de téléchargement Tactica](https://github.com/DyHyugal/pokemon-tactica/releases/download/v1-candidate-63dbd3d9/pokemon-tactica.zip) réunit la même ROM, les informations de version et la checklist de playtest. Après téléchargement, extrais le ZIP pour retrouver le fichier .gba. Les fichiers sont également regroupés sur la [page de cette candidate](https://github.com/DyHyugal/pokemon-tactica/releases/tag/v1-candidate-63dbd3d9).

## Lancer l’aventure

Sur ordinateur, [mGBA](https://mgba.io/downloads.html) permet de lancer la ROM. Installe la version adaptée à ton système, puis ouvre le fichier Tactica .gba depuis le menu de l’émulateur. Si tu utilises un autre appareil, choisis un émulateur compatible Game Boy Advance et charge le même fichier. Le [Guide de jeu](Guide-de-jeu.md) t’accompagne ensuite dans Johto et Kanto.

La ROM de cette V1 est en anglais ; le wiki reste disponible en français et en anglais. Les réglages de difficulté et les aides d’entraînement font partie du jeu : tu n’as pas besoin de télécharger un module supplémentaire pour y accéder.

## La candidate actuelle

Le téléchargement proposé correspond à la candidate **63dbd3d9**, construite depuis la branche **integration/v1**. Elle corrige le plantage de Major Bob (Élecsprint avec Paratonnerre et Rayon Signal) et les sets incompatibles relevés chez les boss et rematches. Les talents et types de 28 couples Méga sont vérifiés par des tests de combat. Elle conserve le choix du rival selon les faiblesses officielles de la catégorie du menu starter et migre les anciennes associations invalides vers une équipe légale, conservée ensuite. Le scaling compte désormais le premier rival vaincu (plancher 18) et chaque victoire centrale de Kanto (80), sans réglage de niveaux à activer. Elle inclut aussi les niveaux des dresseurs ordinaires selon leur zone et la progression, l’Attaque de base de Tranchodon à 130 et les corrections déjà intégrées de combat, de CS et d’interface. Le wiki des évolutions a été synchronisé avec le jeu. Cette version est destinée au playtest et n’est pas encore déclarée stable.

Le premier parcours de validation va jusqu’à Mortimer, puis le playtest complet poursuit l’aventure à travers Johto, Kanto, la deuxième Ligue et Red. Si tu rencontres un problème, indique la version **63dbd3d9**, ta difficulté, ton réglage de cap et les actions qui permettent de le reproduire. Les ajustements et l’équilibrage suivront les retours observés en jeu.

[Tableau complet des niveaux par zone et dresseur (CSV pour Excel)](../assets/tactica-trainer-scaling.csv).

## Retrouver sa sauvegarde

Lorsque tu remplaces la ROM, conserve une copie de ta sauvegarde et de l’ancienne version. Pour reprendre une partie, utilise une sauvegarde effectuée depuis le menu du jeu et sélectionne Continuer. Si ton émulateur associe les fichiers par leur nom, donne le même nom de base à la nouvelle ROM et à une copie de ton fichier .sav, dans le dossier de sauvegardes de l’émulateur. La ROM garde le nom pokemon-tactica.gba ; la sauvegarde associée peut donc rester pokemon-tactica.sav lors des prochaines mises à jour. La mise à jour conserve le format de sauvegarde : elle contrôle la catégorie du starter principal enregistré et garde un rival déjà légal. Une ancienne association Glace/Eau devient Feu ; Électrik/Plante devient Sol. Sauvegarde normalement après chargement pour écrire ce choix dans le fichier ; l’archétype légal restera le même pour la suite de l’aventure.
