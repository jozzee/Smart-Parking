const functions = require('firebase-functions');
//const admin = require('firebase-admin');
//admin.initializeApp(functions.config().firebase);

// // Create and Deploy Your First Cloud Functions
// // https://firebase.google.com/docs/functions/write-firebase-functions
//
// exports.helloWorld = functions.https.onRequest((request, response) => {
//   functions.logger.info("Hello logs!", {structuredData: true});
//   response.send("Hello from Firebase!");
// });

exports.updateParkingCount = functions.database.ref('/status').onWrite((change, context) => {
    //functions.logger.info('Cound Functions: On realtime data write');
    // Only edit data when it is first created.
    // if (change.before.exists()) {
    //     functions.logger.info('Cound Functions: Only edit data when it is first created, Return!!');
    //     return null;
    // }
    // // Exit when the data is deleted.
    // if (!change.after.exists()) {
    //     functions.logger.info('Cound Functions: Exit when the data is deleted, Return!!');
    //     return null;
    // }
    //functions.logger.info('Cound Functions: Ref: ' + change.after.ref);
    //functions.logger.info('Cound Functions: Parrent: ' + change.after.parent);
    var blank = 0;
    var busy = 0;
    change.after.forEach((childSnapshot) => {
        // key will be "ada" the first time and "alan" the second time
        //var key = childSnapshot.key();

        // childData will be the actual contents of the child
        var childData = childSnapshot.val();
        if (childData === "true") {
            blank++;
        } else if (childData === "false") {
            busy++;
        }
    });

    //functions.logger.info('Cound Functions: Blank: ' + blank + ', Bussy: ' + busy);
    change.after.ref.parent.child('/count/blank').set(blank);
    change.after.ref.parent.child('/count/busy').set(busy);
    return change.after.ref.parent.child('/count/all').set(blank + ',' + busy);
});
