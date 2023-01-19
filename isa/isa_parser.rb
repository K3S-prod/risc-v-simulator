require 'yaml'
require 'erb'

def ASSERT(cond)
    raise RuntimeError unless cond
end

class ISA
    def self.init()
        @yaml = YAML.load_file(ARGV[0])
        ParseSignatureDefinition()
        ParseExtensions()
        MapSignaturesToMnemonics()
    end


    def self.signatures_definition
        @signatures_definition
    end

    def self.mnemonics_to_signatures
        @mnemonics_to_signatures
    end

    def self.extensions
        @extensions
    end
    def self.opcodes
        @opcodes
    end

    def self.GetDispatchTable
        disaptch_table = []
        temp_label_str = ""
        opcode_groups.each  do |key, subgroups| 
            key
            subgroups.each do |subgroup|
            subgroup["opc"].each do |opcode|
                subgroup["overloads"].each do |overload|
                    temp_label_str = GetOverloadLabel(opcode, overload)
                    disaptch_table.append("&&" + temp_label_str)
                end
                # fill empty overloads with nullptr:

                (subgroup["overloads"].length .. (opcode_overload_limit - 1)).each do
                    disaptch_table.append("nullptr")
                end
            end
        end end
        return disaptch_table
    end

    private
    def self.ParseSignatureDefinition
        yaml_struct = @yaml["signatures_definition"]
        @signatures_definition = yaml_struct
    end

    def self.CheckSignatureDefinition
        @signatures_definition.each do |signature_name, signature_def|
            total_width = 0
            signature_def.each do |field_name, field_witdth|
                total_width += field_witdth
            end
            raise unless total_width == 32
        end
    end
    
    def self.MapSignaturesToMnemonics
        @mnemonics_to_signatures = {}
        extensions.each do |ext, ext_def|
            ext_def.each do |mnemonic, decode_info|
                raise if mnemonics_to_signatures.key?(mnemonic)
                @mnemonics_to_signatures[mnemonic] = decode_info["signature"]
            end
        end
    end

    def self.ParseExtensions
        @opcodes = {}
        @extensions = @yaml["extensions"]
        extensions.each do |ext, ext_def|
            ext_def.each do |mnemonic, decode_info|
                # All decode info is in string format, need to manualy convert:
                decode_info.each do |field_name, value|
                    next unless field_name != "signature"
                    raise unless value.is_a? String
                    @extensions[ext][mnemonic][field_name] = value.to_i(2)
                end
                raise unless decode_info.key?("opcode")
                raise unless decode_info.key?("signature")
                opc = decode_info["opcode"]
                sign = decode_info["signature"]
                if not @opcodes.key?(opc) then
                    @opcodes[opc] = {}
                end
                if decode_info.size == 2 then
                    raise unless not @opcodes[opc].key?("mnemonic")
                    @opcodes[opc]["mnemonic"] = mnemonic
                else
                    raise unless decode_info.key?("funct_3")
                    funct_3 = decode_info["funct_3"]
                    @opcodes[opc]["field_name"] = "funct_3"
                    if not @opcodes[opc].key?(funct_3) then
                        @opcodes[opc][funct_3] = {}
                    end
                    if decode_info.key?("funct_7") then
                        funct_7 = decode_info["funct_7"]
                        raise if @opcodes[opc][funct_3].key?(funct_7)
                        @opcodes[opc][funct_3]["field_name"] = "funct_7"
                        @opcodes[opc][funct_3][funct_7] = mnemonic
                    elsif decode_info.key?("funct_5") then
                        funct_5 = decode_info["funct_5"]
                        raise if @opcodes[opc][funct_3].key?(funct_5)
                        @opcodes[opc][funct_3]["field_name"] = "funct_5"
                        @opcodes[opc][funct_3][funct_5] = mnemonic
                    elsif decode_info.key?("imm_11_0") then
                        imm_11_0 = decode_info["imm_11_0"]
                        raise if @opcodes[opc][funct_3].key?(imm_11_0)
                        @opcodes[opc][funct_3]["field_name"] = "imm_11_0"
                        @opcodes[opc][funct_3][imm_11_0] = mnemonic
                    else
                        raise if @opcodes[opc][funct_3].key?("mnemonic")
                        @opcodes[opc][funct_3]["mnemonic"] = mnemonic
                    end
                end
            end
        end
    end

end

ISA.init

#TODO: handle arguments properly
n_templates = ARGV.length / 2
(1..n_templates).each do |i|
    template_erb = ARGV[i]
    template_instance = ARGV[i + n_templates]
    template = ERB.new File.read(template_erb), nil, "-"
    File.open(template_instance, 'w') { |file| file.write(template.result) }
end
