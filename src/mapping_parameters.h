#ifndef MAPPING_PARAMETERS_H_
#define MAPPING_PARAMETERS_H_

namespace chromap {

enum MappingOutputFormat {
  MAPPINGFORMAT_UNKNOWN,
  MAPPINGFORMAT_BED,
  MAPPINGFORMAT_TAGALIGN,
  MAPPINGFORMAT_PAF,
  MAPPINGFORMAT_SAM,
  MAPPINGFORMAT_PAIRS
};

struct MappingParameters {
  int error_threshold = 8;
  int match_score = 1;
  int mismatch_penalty = 4;
  std::vector<int> gap_open_penalties = {6, 6};
  std::vector<int> gap_extension_penalties = {1, 1};
  int min_num_seeds_required_for_mapping = 2;
  std::vector<int> max_seed_frequencies = {500, 1000};
  int max_num_best_mappings = 1;
  int max_insert_size = 1000;
  uint8_t mapq_threshold = 30;
  int num_threads = 1;
  int min_read_length = 30;
  int barcode_correction_error_threshold = 1;
  double barcode_correction_probability_threshold = 0.9;
  int multi_mapping_allocation_distance = 0;
  int multi_mapping_allocation_seed = 11;
  int drop_repetitive_reads = 500000;
  bool trim_adapters = false;
  bool remove_pcr_duplicates = false;
  bool remove_pcr_duplicates_at_bulk_level = true;
  bool is_bulk_data = true;
  bool allocate_multi_mappings = false;
  bool only_output_unique_mappings = true;
  bool output_mappings_not_in_whitelist = false;
  bool Tn5_shift = false;
  bool split_alignment = false;
  MappingOutputFormat mapping_output_format = MAPPINGFORMAT_BED;
  bool low_memory_mode = false;
  bool cell_by_bin = false;
  int bin_size = 5000;
  uint16_t depth_cutoff_to_call_peak = 3;
  int peak_min_length = 30;
  int peak_merge_max_length = 30;
  std::string reference_file_path;
  std::string index_file_path;
  std::vector<std::string> read_file1_paths;
  std::vector<std::string> read_file2_paths;
  std::vector<std::string> barcode_file_paths;
  std::string barcode_whitelist_file_path;
  std::string read_format;
  std::string mapping_output_file_path;
  std::string matrix_output_prefix;
  std::string custom_rid_order_path;
  std::string pairs_custom_rid_order_path;
  std::string barcode_translate_table_path;
  bool skip_barcode_check = false;
};

}  // namespace chromap

#endif  // MAPPING_PARAMETERS_H_
