#ifndef OUTPUTTOOLS_H_
#define OUTPUTTOOLS_H_

#include <assert.h>
#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "chromap.h"

namespace chromap {

template <typename MappingRecord>
bool ReadIdLess(const std::pair<uint32_t, MappingRecord> &a, const std::pair<uint32_t, MappingRecord> &b) { 
  return a.second.read_id < b.second.read_id; 
}

struct PAFMapping {
  uint32_t read_id;
  std::string read_name;
  uint16_t read_length;
  uint32_t fragment_start_position;
  uint16_t fragment_length;
  uint8_t mapq : 6, direction : 1, is_unique : 1;
  bool operator<(const PAFMapping& m) const {
    return std::tie(fragment_start_position, fragment_length, mapq, direction, is_unique, read_id, read_length) < std::tie(m.fragment_start_position, m.fragment_length, m.mapq, m.direction, m.is_unique, m.read_id, m.read_length);
  }
  bool operator==(const PAFMapping& m) const {
    return std::tie(fragment_start_position) == std::tie(m.fragment_start_position);
  }
};

struct PairedPAFMapping {
  uint32_t read_id;
  std::string read1_name;
  std::string read2_name;
  uint16_t read1_length;
  uint16_t read2_length;
  uint32_t fragment_start_position;
  uint16_t fragment_length;
  uint16_t positive_alignment_length;
  uint16_t negative_alignment_length;
  uint8_t mapq : 6, direction : 1, is_unique : 1;
  //uint8_t mapq; // least significant bit saves the direction of mapping
  bool operator<(const PairedPAFMapping& m) const {
    return std::tie(fragment_start_position, fragment_length, mapq, direction, is_unique, read_id, positive_alignment_length, negative_alignment_length) < std::tie(m.fragment_start_position, m.fragment_length, m.mapq, m.direction, m.is_unique, m.read_id, m.positive_alignment_length, m.negative_alignment_length);
  }
  bool operator==(const PairedPAFMapping& m) const {
    return std::tie(fragment_start_position, fragment_length) == std::tie(m.fragment_start_position, m.fragment_length);
  }
};

struct MappingWithBarcode {
  uint32_t read_id;
  uint32_t cell_barcode;
  uint32_t fragment_start_position;
  uint16_t fragment_length;
  uint8_t mapq : 6, direction : 1, is_unique : 1;
  //uint8_t mapq;
  bool operator<(const MappingWithBarcode& m) const {
    return std::tie(cell_barcode, fragment_start_position, fragment_length, mapq, direction, is_unique, read_id) < std::tie(m.cell_barcode, m.fragment_start_position, m.fragment_length, m.mapq, m.direction, m.is_unique, m.read_id);
  }
  bool operator==(const MappingWithBarcode& m) const {
    return std::tie(cell_barcode, fragment_start_position) == std::tie(m.cell_barcode, m.fragment_start_position);
  }
};

struct MappingWithoutBarcode {
  uint32_t read_id;
  uint32_t fragment_start_position;
  uint16_t fragment_length;
  //uint8_t mapq;
  uint8_t mapq : 6, direction : 1, is_unique : 1;
  bool operator<(const MappingWithoutBarcode& m) const {
    return std::tie(fragment_start_position, fragment_length, mapq, direction, is_unique, read_id) < std::tie(m.fragment_start_position, m.fragment_length, m.mapq, m.direction, m.is_unique, m.read_id);
  }
  bool operator==(const MappingWithoutBarcode& m) const {
    return std::tie(fragment_start_position) == std::tie(m.fragment_start_position);
  }
};

struct PairedEndMappingWithBarcode {
  uint32_t read_id;
  uint32_t cell_barcode;
  uint32_t fragment_start_position;
  uint16_t fragment_length;
  uint8_t mapq : 6, direction : 1, is_unique : 1;
  //uint8_t mapq;
  uint16_t positive_alignment_length;
  uint16_t negative_alignment_length;
  bool operator<(const PairedEndMappingWithBarcode& m) const {
    return std::tie(cell_barcode, fragment_start_position, fragment_length, mapq, direction, is_unique, read_id, positive_alignment_length, negative_alignment_length) < std::tie(m.cell_barcode, m.fragment_start_position, m.fragment_length, m.mapq, m.direction, m.is_unique, m.read_id, m.positive_alignment_length, m.negative_alignment_length);
  }
  bool operator==(const PairedEndMappingWithBarcode& m) const {
    return std::tie(cell_barcode, fragment_start_position, fragment_length) == std::tie(m.cell_barcode, m.fragment_start_position, m.fragment_length);
  }
};

struct PairedEndMappingWithoutBarcode {
  uint32_t read_id;
  uint32_t fragment_start_position;
  uint16_t fragment_length;
  uint8_t mapq : 6, direction : 1, is_unique : 1;
  //uint8_t mapq;
  uint16_t positive_alignment_length;
  uint16_t negative_alignment_length;
  bool operator<(const PairedEndMappingWithoutBarcode& m) const {
    return std::tie(fragment_start_position, fragment_length, mapq, direction, is_unique, read_id, positive_alignment_length, negative_alignment_length) < std::tie(m.fragment_start_position, m.fragment_length, m.mapq, m.direction, m.is_unique, m.read_id, m.positive_alignment_length, m.negative_alignment_length);
  }
  bool operator==(const PairedEndMappingWithoutBarcode& m) const {
    return std::tie(fragment_start_position, fragment_length) == std::tie(m.fragment_start_position, m.fragment_length);
  }
};

template <typename MappingRecord>
class OutputTools {
 public:
  OutputTools() {}
  virtual ~OutputTools() {}
  inline void InitializeMappingOutput(const std::string &mapping_output_file_path) {
    mapping_output_file_path_ = mapping_output_file_path;
    mapping_output_file_ = fopen(mapping_output_file_path_.c_str(), "w");
    assert(mapping_output_file_ != NULL);
  }
  inline void FinalizeMappingOutput() {
    fclose(mapping_output_file_);
  }
  inline void AppendMappingOutput(const std::string &line) {
    fprintf(mapping_output_file_, "%s", line.data());
  }
  virtual void AppendMapping(uint32_t rid, const SequenceBatch &reference, const MappingRecord &mapping) = 0;
  inline std::string GeneratePAFLine(const SequenceBatch &query_batch, uint32_t query_index, const int query_start, const int query_end, const char relative_strand, const SequenceBatch &target_batch, uint32_t target_index, const int target_start, const int target_end, const int num_matches, const int alignment_length, const int mapping_quality) {
    return std::string(query_batch.GetSequenceNameAt(query_index)) + "\t" + std::to_string(query_batch.GetSequenceLengthAt(query_index)) + "\t" + std::to_string(query_start) + "\t" + std::to_string(query_end) + "\t" + relative_strand + "\t" + std::string(target_batch.GetSequenceNameAt(target_index)) + "\t" + std::to_string(target_batch.GetSequenceLengthAt(target_index)) + "\t" + std::to_string(target_start) + "\t" + std::to_string(target_end) + "\t" + std::to_string(num_matches) + "\t" + std::to_string(alignment_length) + "\t" + std::to_string(mapping_quality) + "\n";
  }
  inline uint32_t GetNumMappings() const {
    return num_mappings_;
  }
  inline std::string Seed2Sequence(uint64_t seed, uint32_t seed_length) const {
    std::string sequence;
    sequence.reserve(seed_length);
    uint64_t mask = 3;
    for (uint32_t i = 0; i < seed_length; ++i) {
      sequence.push_back(SequenceBatch::Uint8ToChar((seed >> ((seed_length - 1 - i) * 2)) & mask));
    }
    return sequence;
  }

  inline void InitializeMatrixOutput(const std::string &matrix_output_prefix) {
    matrix_output_prefix_ = matrix_output_prefix;
    matrix_output_file_ = fopen((matrix_output_prefix_ + "_matrix.mtx").c_str(), "w");
    assert(matrix_output_file_ != NULL);
    peak_output_file_ = fopen((matrix_output_prefix_ + "_peaks.bed").c_str(), "w");
    assert(peak_output_file_ != NULL);
    barcode_output_file_ = fopen((matrix_output_prefix_ + "_barcode.tsv").c_str(), "w");
    assert(barcode_output_file_ != NULL);
  }
  void OutputPeaks(uint32_t bin_size, uint32_t num_sequences, const SequenceBatch &reference) {
    for (uint32_t rid = 0; rid < num_sequences; ++rid) {
      uint32_t sequence_length = reference.GetSequenceLengthAt(rid);
      const char *sequence_name = reference.GetSequenceNameAt(rid);
      for (uint32_t position = 0; position < sequence_length; position += bin_size) {
        fprintf(peak_output_file_, "%s\t%u\t%u\n", sequence_name, position + 1, position + bin_size);
      }
    } 
  }
  void OutputPeaks(uint32_t peak_start_position, uint16_t peak_length, uint32_t rid, const SequenceBatch &reference) {
    const char *sequence_name = reference.GetSequenceNameAt(rid);
    fprintf(peak_output_file_, "%s\t%u\t%u\n", sequence_name, peak_start_position + 1, peak_start_position + peak_length);
  }
  void AppendBarcodeOutput(uint32_t barcode_key) {
    fprintf(barcode_output_file_, "%s-1\n", Seed2Sequence(barcode_key, cell_barcode_length_).data());
  }
  void WriteMatrixOutputHead(uint64_t num_peaks, uint64_t num_barcodes, uint64_t num_lines) {
    fprintf(matrix_output_file_, "%lu\t%lu\t%lu\n", num_peaks, num_barcodes, num_lines);
  }
  void AppendMatrixOutput(uint64_t peak_index, uint64_t barcode_index, uint64_t num_mappings) {
    fprintf(matrix_output_file_, "%lu\t%lu\t%lu\n", peak_index, barcode_index, num_mappings);
  }
  inline void FinalizeMatrixOutput() {
    fclose(matrix_output_file_);
    fclose(peak_output_file_);
    fclose(barcode_output_file_);
  }

 protected:
  std::string mapping_output_file_path_; 
  FILE *mapping_output_file_;
  uint32_t num_mappings_;
  uint32_t cell_barcode_length_ = 16;
  std::string matrix_output_prefix_;
  FILE *peak_output_file_;
  FILE *barcode_output_file_;
  FILE *matrix_output_file_;
};

template <typename MappingRecord>
class BEDOutputTools : public OutputTools<MappingRecord> {
  inline void AppendMapping(uint32_t rid, const SequenceBatch &reference, const MappingRecord &mapping) {
    std::string strand = (mapping.direction & 1) == 1 ? "+" : "-";
    const char *reference_sequence_name = reference.GetSequenceNameAt(rid);
    uint32_t mapping_end_position = mapping.fragment_start_position + mapping.fragment_length;
    this->AppendMappingOutput(std::string(reference_sequence_name) + "\t" + std::to_string(mapping.fragment_start_position) + "\t" + std::to_string(mapping_end_position) + "\tN\t1000\t" + strand + "\n");
  }
};

template <>
class BEDOutputTools<MappingWithBarcode> : public OutputTools<MappingWithBarcode> {
  inline void AppendMapping(uint32_t rid, const SequenceBatch &reference, const MappingWithBarcode &mapping) {
    std::string strand = (mapping.direction & 1) == 1 ? "+" : "-";
    const char *reference_sequence_name = reference.GetSequenceNameAt(rid);
    uint32_t mapping_end_position = mapping.fragment_start_position + mapping.fragment_length;
    this->AppendMappingOutput(std::string(reference_sequence_name) + "\t" + std::to_string(mapping.fragment_start_position) + "\t" + std::to_string(mapping_end_position) + "\t" + Seed2Sequence(mapping.cell_barcode, cell_barcode_length_) +"\n");
  }
};

template <typename MappingRecord>
class BEDPEOutputTools : public OutputTools<MappingRecord> {
  inline void AppendMapping(uint32_t rid, const SequenceBatch &reference, const MappingRecord &mapping) {
    std::string strand = (mapping.direction & 1) == 1 ? "+" : "-";
    const char *reference_sequence_name = reference.GetSequenceNameAt(rid);
    uint32_t mapping_end_position = mapping.fragment_start_position + mapping.fragment_length;
    this->AppendMappingOutput(std::string(reference_sequence_name) + "\t" + std::to_string(mapping.fragment_start_position) + "\t" + std::to_string(mapping_end_position) + "\tN\t1000\t" + strand + "\n");
  }
};

template <>
class BEDPEOutputTools<PairedEndMappingWithBarcode> : public OutputTools<PairedEndMappingWithBarcode> {
  inline void AppendMapping(uint32_t rid, const SequenceBatch &reference, const PairedEndMappingWithBarcode &mapping) {
    std::string strand = (mapping.direction & 1) == 1 ? "+" : "-";
    const char *reference_sequence_name = reference.GetSequenceNameAt(rid);
    uint32_t mapping_end_position = mapping.fragment_start_position + mapping.fragment_length;
    this->AppendMappingOutput(std::string(reference_sequence_name) + "\t" + std::to_string(mapping.fragment_start_position) + "\t" + std::to_string(mapping_end_position) + "\t" + Seed2Sequence(mapping.cell_barcode, cell_barcode_length_) + "\n");
  }
};

template <typename MappingRecord>
class TagAlignOutputTools : public OutputTools<MappingRecord> {
  inline void AppendMapping(uint32_t rid, const SequenceBatch &reference, const MappingRecord &mapping) {
    std::string strand = (mapping.direction & 1) == 1 ? "+" : "-";
    const char *reference_sequence_name = reference.GetSequenceNameAt(rid);
    uint32_t mapping_end_position = mapping.fragment_start_position + mapping.fragment_length;
    this->AppendMappingOutput(std::string(reference_sequence_name) + "\t" + std::to_string(mapping.fragment_start_position) + "\t" + std::to_string(mapping_end_position) + "\tN\t1000\t" + strand + "\n");
  }
};

template <typename MappingRecord>
class PairedTagAlignOutputTools : public OutputTools<MappingRecord> {
  inline void AppendMapping(uint32_t rid, const SequenceBatch &reference, const MappingRecord &mapping) {
    uint8_t strand = mapping.direction & 1;
    uint32_t positive_read_end = mapping.fragment_start_position + mapping.positive_alignment_length;
    uint32_t negative_read_end = mapping.fragment_start_position + mapping.fragment_length;
    uint32_t negative_read_start = negative_read_end - mapping.negative_alignment_length;
    const char *reference_sequence_name = reference.GetSequenceNameAt(rid);
    if (strand == 1) {
      this->AppendMappingOutput(std::string(reference_sequence_name) + "\t" + std::to_string(mapping.fragment_start_position) + "\t" + std::to_string(positive_read_end) + "\tN\t1000\t+\n" + std::string(reference_sequence_name) + "\t" + std::to_string(negative_read_start) + "\t" + std::to_string(negative_read_end) + "\tN\t1000\t-\n");
    } else {
      this->AppendMappingOutput(std::string(reference_sequence_name) + "\t" + std::to_string(negative_read_start) + "\t" + std::to_string(negative_read_end) + "\tN\t1000\t-\n" + std::string(reference_sequence_name) + "\t" + std::to_string(mapping.fragment_start_position) + "\t" + std::to_string(positive_read_end) + "\tN\t1000\t+\n");
    }
  }
};

template <typename MappingRecord>
class PAFOutputTools : public OutputTools<MappingRecord> {
};

template <>
class PAFOutputTools<PAFMapping> : public OutputTools<PAFMapping> {
  inline void AppendMapping(uint32_t rid, const SequenceBatch &reference, const PAFMapping &mapping) {
    const char *reference_sequence_name = reference.GetSequenceNameAt(rid);
    uint32_t reference_sequence_length = reference.GetSequenceLengthAt(rid);
    std::string strand = (mapping.direction & 1) == 1 ? "+" : "-";
    uint32_t mapping_end_position = mapping.fragment_start_position + mapping.fragment_length;
    this->AppendMappingOutput(mapping.read_name + "\t" + std::to_string(mapping.read_length) + "\t" + std::to_string(0) + "\t" + std::to_string(mapping.read_length) + "\t" + strand + "\t" + std::string(reference_sequence_name) + "\t" + std::to_string(reference_sequence_length) + "\t" + std::to_string(mapping.fragment_start_position) + "\t" + std::to_string(mapping_end_position) + "\t" + std::to_string(mapping.read_length) + "\t" + std::to_string(mapping.fragment_length) + "\t" + std::to_string(mapping.mapq) + "\n");
  }
};

template <>
class PAFOutputTools<MappingWithBarcode> : public OutputTools<MappingWithBarcode> {
  inline void AppendMapping(uint32_t rid, const SequenceBatch &reference, const MappingWithBarcode &mapping) {
    const char *reference_sequence_name = reference.GetSequenceNameAt(rid);
    uint32_t reference_sequence_length = reference.GetSequenceLengthAt(rid);
    std::string strand = (mapping.direction & 1) == 1 ? "+" : "-";
    uint32_t mapping_end_position = mapping.fragment_start_position + mapping.fragment_length;
    this->AppendMappingOutput(std::to_string(mapping.read_id) + "\t" + std::to_string(mapping.fragment_length) + "\t" + std::to_string(0) + "\t" + std::to_string(mapping.fragment_length) + "\t" + strand + "\t" + std::string(reference_sequence_name) + "\t" + std::to_string(reference_sequence_length) + "\t" + std::to_string(mapping.fragment_start_position) + "\t" + std::to_string(mapping_end_position) + "\t" + std::to_string(mapping.fragment_length) + "\t" + std::to_string(mapping.fragment_length) + "\t" + std::to_string(mapping.mapq) + "\n");
  }
};

template <>
class PAFOutputTools<MappingWithoutBarcode> : public OutputTools<MappingWithoutBarcode> {
  inline void AppendMapping(uint32_t rid, const SequenceBatch &reference, const MappingWithoutBarcode &mapping) {
    const char *reference_sequence_name = reference.GetSequenceNameAt(rid);
    uint32_t reference_sequence_length = reference.GetSequenceLengthAt(rid);
    std::string strand = (mapping.direction & 1) == 1 ? "+" : "-";
    uint32_t mapping_end_position = mapping.fragment_start_position + mapping.fragment_length;
    this->AppendMappingOutput(std::to_string(mapping.read_id) + "\t" + std::to_string(mapping.fragment_length) + "\t" + std::to_string(0) + "\t" + std::to_string(mapping.fragment_length) + "\t" + strand + "\t" + std::string(reference_sequence_name) + "\t" + std::to_string(reference_sequence_length) + "\t" + std::to_string(mapping.fragment_start_position) + "\t" + std::to_string(mapping_end_position) + "\t" + std::to_string(mapping.fragment_length) + "\t" + std::to_string(mapping.fragment_length) + "\t" + std::to_string(mapping.mapq) + "\n");
  }
};

template <typename MappingRecord>
class PairedPAFOutputTools : public OutputTools<MappingRecord> {
};

template <>
class PairedPAFOutputTools<PairedEndMappingWithBarcode> : public OutputTools<PairedEndMappingWithBarcode> {
  inline void AppendMapping(uint32_t rid, const SequenceBatch &reference, const PairedEndMappingWithBarcode &mapping) {
  }
};

template <>
class PairedPAFOutputTools<PairedEndMappingWithoutBarcode> : public OutputTools<PairedEndMappingWithoutBarcode> {
  inline void AppendMapping(uint32_t rid, const SequenceBatch &reference, const PairedEndMappingWithoutBarcode &mapping) {
  }
};

template <>
class PairedPAFOutputTools<PairedPAFMapping> : public OutputTools<PairedPAFMapping> {
  inline void AppendMapping(uint32_t rid, const SequenceBatch &reference, const PairedPAFMapping &mapping) {
    uint8_t strand = mapping.direction & 1;
    uint32_t positive_read_end = mapping.fragment_start_position + mapping.positive_alignment_length;
    uint32_t negative_read_end = mapping.fragment_start_position + mapping.fragment_length;
    uint32_t negative_read_start = negative_read_end - mapping.negative_alignment_length;
    const char *reference_sequence_name = reference.GetSequenceNameAt(rid);
    uint32_t reference_sequence_length = reference.GetSequenceLengthAt(rid);
    if (strand == 1) {
      this->AppendMappingOutput(mapping.read1_name + "\t" + std::to_string(mapping.read1_length) + "\t" + std::to_string(0) + "\t" + std::to_string(mapping.read1_length) + "\t" + "+" + "\t" + std::string(reference_sequence_name) + "\t" + std::to_string(reference_sequence_length) + "\t" + std::to_string(mapping.fragment_start_position) + "\t" + std::to_string(positive_read_end) + "\t" + std::to_string(mapping.read1_length) + "\t" + std::to_string(mapping.positive_alignment_length) + "\t" + std::to_string(mapping.mapq) + "\n");
      this->AppendMappingOutput(mapping.read2_name + "\t" + std::to_string(mapping.read2_length) + "\t" + std::to_string(0) + "\t" + std::to_string(mapping.read2_length) + "\t" + "-" + "\t" + std::string(reference_sequence_name) + "\t" + std::to_string(reference_sequence_length) + "\t" + std::to_string(negative_read_start) + "\t" + std::to_string(negative_read_end) + "\t" + std::to_string(mapping.read2_length) + "\t" + std::to_string(mapping.negative_alignment_length) + "\t" + std::to_string(mapping.mapq) + "\n");
    } else {
      this->AppendMappingOutput(mapping.read1_name + "\t" + std::to_string(mapping.read1_length) + "\t" + std::to_string(0) + "\t" + std::to_string(mapping.read1_length) + "\t" + "-" + "\t" + std::string(reference_sequence_name) + "\t" + std::to_string(reference_sequence_length) + "\t" + std::to_string(negative_read_start) + "\t" + std::to_string(negative_read_end) + "\t" + std::to_string(mapping.read1_length) + "\t" + std::to_string(mapping.negative_alignment_length) + "\t" + std::to_string(mapping.mapq) + "\n");
      this->AppendMappingOutput(mapping.read2_name + "\t" + std::to_string(mapping.read2_length) + "\t" + std::to_string(0) + "\t" + std::to_string(mapping.read2_length) + "\t" + "+" + "\t" + std::string(reference_sequence_name) + "\t" + std::to_string(reference_sequence_length) + "\t" + std::to_string(mapping.fragment_start_position) + "\t" + std::to_string(positive_read_end) + "\t" + std::to_string(mapping.read2_length) + "\t" + std::to_string(mapping.positive_alignment_length) + "\t" + std::to_string(mapping.mapq) + "\n");
    }
  }
};
} // namespace chromap

#endif // OUTPUTTOOLS_H_
