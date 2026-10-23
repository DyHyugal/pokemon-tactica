# Pokémon et capacités custom

`data/spec/pokemon_balance.json` contient 26 entrées d'espèces et 16 entrées de learnset migrées du dataset custom v4 validé. Appliquer uniquement ces modifications, contrôler leur présence dans le code et l'accès effectif aux capacités ; conserver les autres formes non visées. En particulier : Élekable **Électrik/Combat, Sans Limite**, Tranchodon **Dragon/Acier**, Jungko **Plante/Dragon physique**, Majaspic **Plante/Dragon**, famille Salamèche **Feu/Dragon**. Les vieilles idées Électrik/Sol et Ténèbres/Acier sont abandonnées.

Correction postérieure au dataset v4 : Dracaufeu Atk `100 → 110`, Méga-Dracaufeu X `146 → 156`. Méga-Dracaufeu Y **reste à 120 Atk selon le v4**, sans +10 supplémentaire. Sa spécialisation reste spéciale. Ne pas propager mécaniquement tout nouveau delta à toutes les formes Méga.

Correspondances officielles importantes pour la traduction : `MOVE_WAVE_CRASH` = Aquatacle, et non Aqua Tail ; `MOVE_FLIP_TURN` = Eau Revoir. Conserver les seize changements de capacités de la source et ne pas réintroduire les anciens candidats non validés.

Hydragon reçoit aussi Draco-Griffe comme capacité compatible afin de rendre légal son set du match retour du Maître.

Tranchodon a **130 en Attaque** (147 − 17, demande owner du 23 octobre 2026). Son type Dragon/Acier, sa Vitesse 110 et ses capacités restent inchangés. Les statistiques et types custom du Pokédex sont synchronisés depuis le JSON par `tools/sync_tactica_species_balance.py`.
