import 'package:firebase_core/firebase_core.dart';
import 'package:flutter/material.dart';
import 'package:session2/fetch.dart';
import 'package:session2/write.dart';


void main() async {
  WidgetsFlutterBinding.ensureInitialized();

  FirebaseOptions firebaseOptions = const FirebaseOptions(
      apiKey: "AIzaSyCDIX_8R5TSoMmUHrZVrufLrAC32vzNUnw",
      authDomain: "odda-5c342.firebaseapp.com",
      databaseURL: "https://odda-5c342-default-rtdb.firebaseio.com",
      projectId: "odda-5c342",
      storageBucket: "odda-5c342.appspot.com",
      messagingSenderId: "1039977707167",
      appId: "1:1039977707167:web:6d2cd5c77b5a55063594aa",
      measurementId: "G-GZYJ6CSGJ7"
  );

  // FirebaseOptions firebaseOptions = const FirebaseOptions(
  //     apiKey: "AIzaSyCDIX_8R5TSoMmUHrZVrufLrAC32vzNUnw",
  //     authDomain: "odda-5c342.firebaseapp.com",
  //     databaseURL: "https://odda-5c342-default-rtdb.firebaseio.com",
  //     projectId: "odda-5c342",
  //     storageBucket: "odda-5c342.appspot.com",
  //     messagingSenderId: "1039977707167",
  //     appId: "1:1039977707167:android:caa4e89d5609b6c13594aa",
  //     measurementId: "G-GZYJ6CSGJ7"
  // );

  await Firebase.initializeApp(
    options: firebaseOptions
  );


  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      debugShowCheckedModeBanner: false,
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: const MyHomePage(),
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({Key? key}) : super(key: key);

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text(
          'Gas Station (Circle K)',
          style: TextStyle(
            fontSize: 30,
            fontWeight: FontWeight.w700,
          ),
        ),
        centerTitle: true,
        backgroundColor: Color(0xffdc143c),
      ),
      body: Column(
        children: <Widget>[
          Expanded(
            child: Container(
              width: double.infinity,
              decoration: const BoxDecoration(
                image: DecorationImage(
                  image: AssetImage('assets/OIP.jpg'),
                  fit: BoxFit.cover,
                ),
              ),
              child: Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  SizedBox(height: MediaQuery.of(context).size.height * 0.15),
                  MaterialButton(
                    onPressed: () {
                      Navigator.push(
                        context,
                        MaterialPageRoute(builder: (context) => const WriteData()),
                      );
                    },
                    child: const Text(
                      'Enter amount',
                      style: TextStyle(
                        fontSize: 20,
                        fontWeight: FontWeight.w500,
                      ),
                    ),
                    color: Color(0xff0c58ee),
                    textColor: Colors.white,
                    height: 50,
                    minWidth: MediaQuery.of(context).size.width * 0.6,
                  ),
                  SizedBox(height: 10),
                  MaterialButton(
                    onPressed: () {
                      Navigator.push(
                        context,
                        MaterialPageRoute(builder: (context) => const FetchData()),
                      );
                    },
                    child: const Text(
                      'Show Logs',
                      style: TextStyle(
                        fontSize: 20,
                        fontWeight: FontWeight.w500,
                      ),
                    ),
                    color: Color(0xff0c58ee),
                    textColor: Colors.white,
                    height: 50,
                    minWidth: MediaQuery.of(context).size.width * 0.6,
                  ),
                ],
              ),
            ),
          ),

        ],
      ),
    );
  }
}
