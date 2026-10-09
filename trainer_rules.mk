# party files are run through trainerproc, which is a tool that converts party data to an output file
# matching the current trainer .h formatting

AUTO_GEN_TARGETS += src/data/trainers.h
AUTO_GEN_TARGETS += src/data/trainers_frlg.h
AUTO_GEN_TARGETS += src/data/trainers_hns.h
AUTO_GEN_TARGETS += src/data/battle_partners.h
AUTO_GEN_TARGETS += test/battle/trainer_control.h
AUTO_GEN_TARGETS += test/battle/partner_control.h
AUTO_GEN_TARGETS += src/data/debug_trainers.h

%.h: %.party $(TRAINERPROC)
	$(CPP) $(CPPFLAGS) -traditional-cpp - < $< | $(TRAINERPROC) -o $@ -i $< -

# Real HnS story teams are included in the test ROM outside IDs reserved by
# trainer_control.party. This prevents cap tests from testing empty rosters.
AUTO_GEN_TARGETS += test/battle/tactica_story_trainers.h
test/battle/tactica_story_trainers.h: src/data/trainers_hns.h tools/generate_tactica_story_test_trainers.py
	python3 tools/generate_tactica_story_test_trainers.py
