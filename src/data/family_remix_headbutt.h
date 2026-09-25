// Tactica Headbutt tables use the same four real 30/30/30/10 slots as every
// other HnS encounter method.
static const struct WildPokemon sFamilyHeadbuttIlexForest[] =
{
    {24, 27, SPECIES_PINECO},
    {24, 27, SPECIES_NINCADA},
    {24, 27, SPECIES_PARAS},
    {24, 27, SPECIES_HERACROSS},
};

static const struct WildPokemon sFamilyHeadbuttNationalParkContest[] =
{
    {26, 29, SPECIES_HELIOPTILE},
    {26, 29, SPECIES_TOEDSCOOL},
    {26, 29, SPECIES_RUFFLET},
    {26, 29, SPECIES_PUMPKABOO},
};

static const struct WildPokemon sFamilyHeadbuttNationalPark[] =
{
    {26, 29, SPECIES_SNOM},
    {26, 29, SPECIES_SWABLU},
    {26, 29, SPECIES_FOONGUS},
    {26, 29, SPECIES_CAPSAKID},
};

static const struct WildPokemon sFamilyHeadbuttViridianForest[] =
{
    {70, 84, SPECIES_SCYTHER},
    {70, 84, SPECIES_CENTISKORCH},
    {70, 84, SPECIES_KLEAVOR},
    {70, 84, SPECIES_VIKAVOLT},
};

struct TacticaHeadbuttEncounter
{
    u8 mapGroup;
    u8 mapNum;
    struct WildPokemonInfo info;
};

static const struct TacticaHeadbuttEncounter sTacticaHeadbuttEncounters[] =
{
    {MAP_GROUP(MAP_ILEX_FOREST_HNS), MAP_NUM(MAP_ILEX_FOREST_HNS), {50, sFamilyHeadbuttIlexForest}},
    {MAP_GROUP(MAP_NATIONAL_PARK_BUG_CONTEST_HNS), MAP_NUM(MAP_NATIONAL_PARK_BUG_CONTEST_HNS), {50, sFamilyHeadbuttNationalParkContest}},
    {MAP_GROUP(MAP_NATIONAL_PARK_NORMAL_HNS), MAP_NUM(MAP_NATIONAL_PARK_NORMAL_HNS), {50, sFamilyHeadbuttNationalPark}},
    {MAP_GROUP(MAP_VIRIDIAN_FOREST_HNS), MAP_NUM(MAP_VIRIDIAN_FOREST_HNS), {50, sFamilyHeadbuttViridianForest}},
};
