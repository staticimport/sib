
ACT_HOME = ENV['ACT_HOME']

require "#{ACT_HOME}/lib/act/gcc.rb"

def act
  bdr = Act::GccBuilder.new({
    :warn => ['all','error']
  })
  cpp_files = Dir.glob('src/test/**/*.cpp')
  bdr.static_lib('libsibtest.a', cpp_files, lambda {|src| 'build/'})
end

