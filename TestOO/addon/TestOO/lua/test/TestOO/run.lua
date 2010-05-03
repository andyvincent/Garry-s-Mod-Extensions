
local tests = {}
local currentTest = nil

function testFailed(reason)
  print(" *** FAILURE ***")
  if (reason) then
    print(reason)
  else
    print("Unknown reason")
  end
end

function testSuccess()
  print(" *** SUCCESS ***")
  nextTest()
end

function nextTest()
  // find the next test to run
  currentTest = next(tests, currentTest)
  if (!currentTest) then
    print(" *** FINISHED RUNNING TESTS ***")
    return;
  end
  
  local currentTestInfo = tests[currentTest]
  
  // print the test name and run the function
  print("==[ " .. currentTestInfo.name .. " ]==============================")
  local status, result = pcall( currentTestInfo.test )
  if (!status) then
    // somekind of error, print it
    testFailed(result)
  end
end

function addTest(_name, _test)
  table.insert( tests, {name=_name, test = _test } );
end

timer.Simple(0, nextTest)

////////////////////////////////////////////////////////////////////////////////////////////////////////
// Module test code goes here!
////////////////////////////////////////////////////////////////////////////////////////////////////////

require("testoo")

addTest("Creation", function()
  local newObject = testoo.create()
  print(newObject) // Should print "[Test:<address>]"
  // Note: GC should take care of destruction!
  testSuccess()
end )

addTest("Destruction", function()
  local newObject = testoo.create()
  print(newObject)   // Should print "[Test:<address>]"
  newObject:delete() // Force destruction ourselves
  print(newObject)   // Should print "[NULL Object]"
  testSuccess()
end )

addTest("Function", function()
  local newObject = testoo.create()
  print(newObject)        // Should print "[Test:<address>]"
  newObject:normalTest()  // Should print "Testing..Testing...1...2...3..." via the object
  
  testSuccess()
end )

addTest("Poll", function()
  local newObject = testoo.create()
  print(newObject)  // Should print "[Test:<address>]"
  newObject:poll()  // Should print "Poll ... <number>" via the object

// Between now and then 5 second delay, the poll function will be called every frame/server tick

  timer.Simple(5, function() newObject:delete() testSuccess() end ) // delete the object after 5 seconds
end )

addTest("Callback", function()
  local newObject = testoo.create()
  print(newObject)        // Should print "[Test:<address>]"

  function newObject:onCallback(STRING, TIME)
    print("newObject:onCallback", self, STRING, TIME)
  end

  newObject:callbackTest()  // Should run the callback function above (with Running callback.. ... done
                            // around the output

  function functionTest2(OBJECT, STRING, TIME)
    print("functionTest2", OBJECT, STRING, TIME)
  end
  newObject.onCallback = functionTest2

  newObject:callbackTest()  // Should run the callback function above (with Running callback.. ... done
                            // around the output

  newObject.onCallback = function(OBJECT, STRING, TIME)
    print("anon-function", OBJECT, STRING, TIME)
  end

  newObject:callbackTest()  // Should run the callback function above (with Running callback.. ... done
                            // around the output
  testSuccess()
end )
