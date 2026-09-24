# Boss, Méga et IA

Le roster de référence est `data/spec/bosses.json`. Il reprend le dataset de boss v8 avec les décisions ultérieures appliquées explicitement. Un niveau `null` signifie dynamique, et non manquant. Toute divergence avec les fichiers compilés doit être signalée dans l'audit avant correction.

## Structure et difficulté

Albert 3, Hector 4, Blanche et après 6 Pokémon. Même roster, moves, talents, objets, natures et niveaux en NORMAL/HARD ; HARD : IV 31 pour boss fixes, EV au plus 252 par statistique et 510 au total, IA stratégique équitable. NORMAL conserve l'optimisation native. Pour les attaquants, privilégier nature Vitesse ou attaque utile en réduisant l'attaque inutilisée, sauf justification de rôle. Au plus deux soins s'ils sont prévus, du type d'objet effectivement accessible au stade concerné.

L'IA peut gérer suicide lead, climat/terrain, murs, pivots, setup et plusieurs carries. Les hazards ne sont pas obligatoires dans toutes les équipes. Elle ne lit pas des informations cachées du joueur et ne prédit pas omniscientement son choix. Gestion des objets Choix, capacités de statut, météo, écrans et bascule vers un autre rôle lorsqu'un plan ne marche plus. La logique détaillée s'appuie sur les rôles des équipes et les tests de combat, sans donner à HARD un roster différent.

## Identités confirmées

Frédo : Feunard d'Alola, Momartik, Blizzaroi, Glaivodo, Galvagla, Cochignon. Feunard d'Alola pose Neige et Voile Aurore puis peut se retirer ; Cochignon Évoluroc/Isograisse/Malédiction. **Pas de Mammochon.** Jasmine : Noacier, Gromago, Corvaillus, Pondralugon, Magnézone, **Méga-Galeking**, sans tempête de sable. Pierre garde Méga-Steelix. Ondine Pluie et Méga-Staross si effectivement jouable ; Major Bob Champ Électrifié et Méga-Raichu si jouable ; Erika Champ Herbu ; Auguste Soleil ; Morgane Distorsion ; Jeannine Poison/contrôle ; Blue hyper offense. Clément Méga-Gardevoir, Morgane Méga-Alakazam. Rocket : Proton hyper offense/Poison ; Petrel nuisance ; Ariana contrôle/setup ; Archer offense équilibrée. Les autres équipes et membres du Conseil 4 sont précisés par le JSON, et non supprimés faute d'être énumérés ici.

Les Méga des boss majeurs doivent être uniques ; en cas de doublon, le personnage le plus lié au Pokémon garde sa Méga. L'autre reçoit une Méga disponible et cohérente avec son type, climat, terrain et vitesse. Le set de Jasmine doit partir d'un **vrai set de Méga-Galeking**, pas d'un Galeking normal renommé. Candidat de référence : Aggronite, Malédiction, Tacle Lourd, Repos, Blabla Dodo, nature Prudente, EV 252 PV / 4 Déf / 252 Déf. Spé, talent après Méga selon le build. Ce set est un set de Méga-Galeking documenté ; aucun prétendu set critique populaire n'a été identifié. Vérifier les noms, capacités, talent et objet dans le moteur avant de le figer.

## Points du dataset corrigés

Voltali du Maître en rematch : Modeste, Vive-Attaque + Change Éclair, rôle spécial. Hyporoi garde Lentilscope. Corboss avec Orbe Vie là où le roster l'attribue. Magnézone spécial : Rayon Signal au lieu de Big Splash. Noadkoko spécial : Pouvoir Antique au lieu de Poudre Dodo. Rocket prend le dernier cap champion/rival +2, **jamais** le Rocket précédent +2.
