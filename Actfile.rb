
require "#{ACT_HOME}/lib/act/gcc.rb"
require "#{ACT_HOME}/lib/act/task.rb"

def act
  # Init dirs for output
  system('mkdir -p install/include install/lib')
  bdr = Act::GccBuilder.new({
    :misc => ['std=c++0x'],
    :warn => ['all','error']
  })
  hpp_files = Dir.glob('src/main/**/*.hpp')
  cpp_files = Dir.glob('src/main/**/*.cpp')
  #lib_task = bdr.static_lib('install/lib/libsibtest.a', 
  #                          cpp_files, lambda {|src| 'build/main'})

  bdr.copy(hpp_files, [], 
           lambda {|f| "install/include/#{File.basename(f)}"})
end

#TESTS = ['dynamic_ring','ring']
TESTS = ['ring']
def test
  act()
  system('mkdir -p test')
  bdr = Act::GccBuilder.new({
    :include => ['install/include','/usr/local/include/'],
    :libdir => ['install/lib','/usr/local/lib'],
    :lib => ['boost_unit_test_framework-mt'],
    :misc => ['std=c++0x', 'pedantic'],
    :warn => ['all','error']})
  TESTS.each do |test|
    exe = bdr.executable("src/test/#{test}_test.cpp", "test/#{test}_test")
    run_task = Act::Task.new do
      puts "---> testing #{test}"
      puts %x{#{"test/#{test}_test --report_level=detailed " +
                "--show_progress=true --build_info=true 2>&1"}}
    end
    run_task.depends.add exe
  end
end

