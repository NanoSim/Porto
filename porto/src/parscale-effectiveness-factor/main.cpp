#include <Soft>
#include <QtCore>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <string>
#include <vector>

#include "effectivenessfactor.hxx"
#include "effectivereactionparameters.hxx"

QJsonDocument readJson(QString filename)
{
   // Read from JSON
   QFile f(filename);
   if (!f.open(QIODevice::ReadOnly))
   {
      QTextStream(stderr) << "Error: Could not read " << filename << endl;
   }

   return QJsonDocument::fromJson(f.readAll());
}

struct EffectivenessFactor
{
   std::string name;
   std::string functionalForm;
   std::vector<double> parameters;
   std::vector<std::string> arguments;
};

EffectivenessFactor toEffectivenessFactor(QJsonDocument doc)
{
   const std::string name = doc.object()["name"].toString().toStdString();
   const std::string functionalForm = doc.object()["functionalForm"].toString().toStdString();

   const QList<QVariant> paramList = doc.object()["parameters"].toArray().toVariantList();
   const QList<QVariant> argumentList = doc.object()["arguments"].toArray().toVariantList();

   std::vector<double> parameters;
   for (const auto &p : paramList)
   {
      bool ok = false;
      double value = p.toReal(&ok);
      if (!ok)
      {
         throw std::runtime_error("Unable to convert parameter value to double");
      }
      parameters.push_back(value);
   }

   std::vector<std::string> arguments;
   for (const auto &a : argumentList)
   {
      arguments.push_back(a.toString().toStdString());
   }

   return {
       name,
       functionalForm,
       parameters,
       arguments};
}

struct EffectiveReactionParameters
{
   std::string name;
   int multiplyByParticleVolumeFraction;
   std::string functionalForm;
   std::vector<double> parameters;
   std::vector<std::string> arguments;
};

EffectiveReactionParameters toEffectiveReactionParameters(QJsonDocument doc)
{
   const std::string name = doc.object()["name"].toString().toStdString();
   const std::string functionalForm = doc.object()["functionalForm"].toString().toStdString();
   const int multiplyByParticleVolumeFraction = doc.object()["multiplyByParticleVolumeFraction"].toInt();

   const QList<QVariant> paramList = doc.object()["parameters"].toArray().toVariantList();
   const QList<QVariant> argumentList = doc.object()["arguments"].toArray().toVariantList();

   std::vector<double> parameters;
   for (const auto &p : paramList)
   {
      bool ok = false;
      double value = p.toReal(&ok);
      if (!ok)
      {
         throw std::runtime_error("Unable to convert parameter value to double");
      }
      parameters.push_back(value);
   }

   std::vector<std::string> arguments;
   for (const auto &a : argumentList)
   {
      arguments.push_back(a.toString().toStdString());
   }

   return {
       name,
       multiplyByParticleVolumeFraction,
       functionalForm,
       parameters,
       arguments};
}

int main(int argc, char **argv)
{
   ::soft::init(argc, argv);
   if (argc < 4)
   {
      QTextStream(stderr) << "Usage " << argv[0] << " <effectiveness_factor.json> <effective_reaction_parameters.json> <uuid>" << endl;
      return 1;
   }

   auto &effectivenessFactorFilename = argv[1];         // i.e. effectiveness_factor.json
   auto &effectiveReactionParametersFilename = argv[2]; // i.e. effective_reaction_parameters.json
   auto &uuid = argv[3];

   // -------------------------
   if (!QFileInfo::exists(effectivenessFactorFilename))
   {
      QTextStream(stderr) << "Error: Could not open " << effectivenessFactorFilename << endl;
      return 1;
   }

   QJsonDocument docEffectivenessFactor = readJson(effectivenessFactorFilename);
   EffectivenessFactor ef;
   try
   {
      ef = toEffectivenessFactor(docEffectivenessFactor);
   }
   catch (std::runtime_error &e)
   {
      QTextStream(stderr) << "Error: Unable to parse json file: '" << effectivenessFactorFilename << "': " << e.what() << endl;
      return 1;
   }
   const std::string efname = "effectivenessFactor";
   if (ef.name != efname)
   {
      QTextStream(stderr) << "Error: In '" << effectivenessFactorFilename << "', expected field 'name' == '" << QString::fromStdString(efname) << "', found '" << QString::fromStdString(ef.name) << "'" << endl;
      return 1;
   }
   soft::Effectivenessfactor ef_e(ef.parameters.size(), ef.arguments.size());
   ef_e.functionalForm = ef.functionalForm;
   ef_e.parameters = ef.parameters;
   ef_e.arguments = ef.arguments;

   // -------------------------
   if (!QFileInfo::exists(effectiveReactionParametersFilename))
   {
      QTextStream(stderr) << "Error: Could not open " << effectiveReactionParametersFilename << endl;
      return 1;
   }

   QJsonDocument docEffectiveReactionparameters = readJson(effectiveReactionParametersFilename);
   EffectiveReactionParameters erp;
   try
   {
      erp = toEffectiveReactionParameters(docEffectiveReactionparameters);
   }
   catch (std::runtime_error &e)
   {
      QTextStream(stderr) << "Error: Unable to parse json file: '" << effectiveReactionParametersFilename << "': " << e.what() << endl;
      return 1;
   }
   const std::string erpname = "effectiveReactionparameters";
   if (erp.name != erpname)
   {
      QTextStream(stderr) << "Error: In '" << effectivenessFactorFilename << "', expected field 'name' == '" << QString::fromStdString(erpname) << "', found '" << QString::fromStdString(erp.name) << "'" << endl;
      return 1;
   }
   soft::Effectivereactionparameters erp_e(erp.parameters.size(), erp.arguments.size());
   erp_e.multiplyByParticleVolumeFraction = erp.multiplyByParticleVolumeFraction;
   erp_e.functionalForm = erp.functionalForm;
   erp_e.parameters = erp.parameters;
   erp_e.arguments = erp.arguments;

   // --------------------------
   soft::Collection collection(uuid);
   collection.attachEntity("Effectivenessfactor", &ef_e);
   collection.attachEntity("Effectivereactionparameters", &erp_e);

   soft::Storage storage("mongo2", "mongodb://localhost", "db=porto;coll=demo");
   storage.save(&collection);

   return 0;
}
