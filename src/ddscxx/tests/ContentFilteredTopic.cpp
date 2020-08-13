/*
 * Copyright(c) 2006 to 2018 ADLINK Technology Limited and others
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v. 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
 * v. 1.0 which is available at
 * http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause
 */
#include "dds/dds.hpp"
#include "dds/ddscxx/test.h"
#include "Space_DCPS.hpp"


/**
 * Fixture for the DataReader tests
 */
class ddscxx_ContentFilteredTopic : public ::testing::Test
{
public:
    dds::domain::DomainParticipant participant;
    dds::sub::Subscriber subscriber;
    dds::pub::Publisher publisher;
    dds::topic::Topic<Space::Type3> topic;
    dds::topic::ContentFilteredTopic<Space::Type3> cfTopic;
    dds::sub::DataReader<Space::Type3> reader;
    dds::pub::DataWriter<Space::Type3> writer;

    dds::sub::status::DataState read_not_new;

    ddscxx_ContentFilteredTopic() :
        participant(dds::core::null),
        subscriber(dds::core::null),
        publisher(dds::core::null),
        topic(dds::core::null),
        cfTopic(dds::core::null),
        reader(dds::core::null),
        writer(dds::core::null),
        read_not_new(dds::sub::status::SampleState::read(),
                     dds::sub::status::ViewState::not_new_view(),
                     dds::sub::status::InstanceState::alive())
    {
    }

    virtual void set_filter_function() = 0;

    void SetUp()
    {
//        printf("Waiting for debugger to attach.....\n");
//        Sleep(8000);
        this->participant = dds::domain::DomainParticipant(org::eclipse::cyclonedds::domain::default_id());
        ASSERT_NE(this->participant, dds::core::null);

        this->publisher = dds::pub::Publisher(this->participant);
        ASSERT_NE(this->publisher, dds::core::null);

        this->subscriber = dds::sub::Subscriber(this->participant);
        ASSERT_NE(this->subscriber, dds::core::null);

        dds::topic::qos::TopicQos topic_qos = this->participant.default_topic_qos();
        topic_qos << dds::core::policy::Reliability::Reliable() <<
                     dds::core::policy::History::KeepAll();
        this->topic = dds::topic::Topic<Space::Type3>(this->participant,
                                                      "unfiltered_test_topic",
                                                      topic_qos);
        ASSERT_NE(this->topic, dds::core::null);

        this->cfTopic = dds::topic::ContentFilteredTopic<Space::Type3>(this->topic, "filtered_test_topic", dds::topic::Filter("1=1"));
        ASSERT_NE(this->topic, dds::core::null);

        this->set_filter_function();

        dds::pub::qos::DataWriterQos writer_qos = this->publisher.default_datawriter_qos();
        writer_qos = this->topic.qos();
        this->writer = dds::pub::DataWriter<Space::Type3>(this->publisher,
                                                          this->topic,
                                                          writer_qos);
        ASSERT_NE(this->writer, dds::core::null);

        dds::sub::qos::DataReaderQos reader_qos = this->subscriber.default_datareader_qos();
        reader_qos = this->topic.qos();
        this->reader = dds::sub::DataReader<Space::Type3>(this->subscriber, this->cfTopic, reader_qos);
        ASSERT_NE(this->reader, dds::core::null);
    }

    std::vector<Space::Type3> CreateSamples(
                     int32_t instances_start,
                     int32_t instances_end,
                     int32_t samples_start,
                     int32_t samples_end)
    {
        std::vector<Space::Type3> samples;
        for (int32_t i = instances_start; i <= instances_end; i++) {
            for (int32_t s = samples_start; s <= samples_end; s++) {
                std::stringstream stream;
                stream << "name" << i+s+1;
                std::vector<int32_t> numbers = {i+s+2, i+s+3};
                samples.push_back(Space::Type3(i, stream.str(), numbers));
            }
        }
        return samples;
    }

    void WriteData(const std::vector<Space::Type3>& samples)
    {
        for (size_t i = 0; i < samples.size(); i++) {
            this->writer.write(samples[(size_t)i]);
        }
    }

    void CheckData (
        const dds::sub::LoanedSamples<Space::Type3> &samples,
        const std::vector<Space::Type3>& test_data,
        const dds::sub::status::DataState& test_state =
                dds::sub::status::DataState(dds::sub::status::SampleState::not_read(),
                dds::sub::status::ViewState::new_view(),
                dds::sub::status::InstanceState::alive()))
    {
        unsigned long count = 0UL;
        ASSERT_EQ(samples.length(), test_data.size());
        dds::sub::LoanedSamples<Space::Type3>::const_iterator it;
        for (it = samples.begin(); it != samples.end(); ++it, ++count) {
            const Space::Type3& data = it->data();
            const dds::sub::SampleInfo& info = it->info();
            const dds::sub::status::DataState& state = info.state();
            ASSERT_EQ(data, test_data[count]);
            ASSERT_EQ(state.view_state(), test_state.view_state());
            ASSERT_EQ(state.sample_state(), test_state.sample_state());
            ASSERT_EQ(state.instance_state(), test_state.instance_state());
        }
    }

    void
    CheckData (
        const std::vector<dds::sub::Sample<Space::Type3> > &samples,
        const std::vector<Space::Type3>& test_data,
        const dds::sub::status::DataState& test_state =
                dds::sub::status::DataState(dds::sub::status::SampleState::not_read(),
                dds::sub::status::ViewState::new_view(),
                dds::sub::status::InstanceState::alive()))
    {
        unsigned long count = 0UL;
        std::vector<dds::sub::Sample<Space::Type3> >::const_iterator it;
        ASSERT_EQ(samples.size(), test_data.size());
        for (it = samples.begin(); it != samples.end(); ++it, ++count) {
            const Space::Type3& data = it->data();
            const dds::sub::SampleInfo& info = it->info();
            const dds::sub::status::DataState& state = info.state();
            ASSERT_EQ(data, test_data[count]);
            ASSERT_EQ(state.view_state(), test_state.view_state());
            ASSERT_EQ(state.sample_state(), test_state.sample_state());
            ASSERT_EQ(state.instance_state(), test_state.instance_state());
        }
    }

    void TearDown()
    {
        this->writer = dds::core::null;
        this->reader = dds::core::null;
        this->topic = dds::core::null;
        this->publisher = dds::core::null;
        this->subscriber = dds::core::null;
        this->participant = dds::core::null;
    }

};

class ddscxx_LambdaContentFilteredTopic1 : public ddscxx_ContentFilteredTopic
{
public:
    void set_filter_function()
    {
        this->cfTopic->filter_function([](const Space::Type3 &sample)
            {
                return (sample.long_1() > 5);
            });
    }
};

class ddscxx_LambdaContentFilteredTopic2 : public ddscxx_ContentFilteredTopic
{
public:
    void set_filter_function()
    {
        this->cfTopic->filter_function([](const Space::Type3 &sample)
            {
                return (sample.name() < "name4");
            });
    }
};

class ddscxx_OperatorContentFilteredTopic : public ddscxx_ContentFilteredTopic
{
private:
    class OperatorFilter {
    public:
        bool operator() (const Space::Type3 &sample) {
            return (sample.long_1() < 1 || sample.name() > "name5");
        }
    };
    OperatorFilter myFilter;

public:
    void set_filter_function()
    {
        this->cfTopic->filter_function(myFilter);
    }
};


/**
 * Tests
 */

DDSCXX_TEST_F(ddscxx_LambdaContentFilteredTopic1, FilterKey)
{
    dds::sub::LoanedSamples<Space::Type3> read_samples;
    std::vector<Space::Type3> write_samples, expected_samples;

    /* Get test data. */
    write_samples = this->CreateSamples(0, 6,  /* instances */
                                        0, 0); /* samples   */
    expected_samples = this->CreateSamples(6, 6,  /* instances */
                                           0, 0); /* samples   */

    /* Write test data. */
    this->WriteData(write_samples);

    /* Read through the Selector. */
    read_samples = this->reader.read();

    /* Check result. */
    this->CheckData(read_samples, expected_samples);

    /* A second time should work as well. */
    read_samples = this->reader.read();
    this->CheckData(read_samples, expected_samples, ddscxx_ContentFilteredTopic::read_not_new);
}

DDSCXX_TEST_F(ddscxx_LambdaContentFilteredTopic2, FilterString)
{
    dds::sub::LoanedSamples<Space::Type3> read_samples;
    std::vector<Space::Type3> write_samples, expected_samples;

    /* Get test data. */
    write_samples = this->CreateSamples(1, 5,  /* instances */
                                        0, 0); /* samples   */
    expected_samples = this->CreateSamples(1, 2,  /* instances */
                                           0, 0); /* samples   */

    /* Write data. */
    this->WriteData(write_samples);

    /* Read through the Selector. */
    read_samples = this->reader.read();

    /* Check result. */
    this->CheckData(read_samples, expected_samples);
}

DDSCXX_TEST_F(ddscxx_OperatorContentFilteredTopic, FilterKeyAndString)
{
  dds::sub::LoanedSamples<Space::Type3> read_samples;
  std::vector<Space::Type3> write_samples, expected_samples;

  /* Get test data. */
  write_samples = this->CreateSamples(0, 6,  /* instances */
                                      0, 0); /* samples   */
  expected_samples = this->CreateSamples(0, 6,  /* instances */
                                         0, 0); /* samples   */

  /* Erase the part that will not come through the filter. */
  auto it_start = expected_samples.begin(), it_end = expected_samples.begin();
  it_start++;
  it_end += 5;
  expected_samples.erase(it_start, it_end);


  /* Write data. */
  this->WriteData(write_samples);

  /* Read through the Selector. */
  read_samples = this->reader.read();

  /* Check result. */
  this->CheckData(read_samples, expected_samples);
}
