import clsx from 'clsx';
import CodeBlock from '@theme/CodeBlock';
import Heading from '@theme/Heading';
import styles from './styles.module.css';

const ioFormats =
`void InputFormat() {
    LINE(N);
    LINES(A) % SIZE(N);
}

void OutputFormat() {
    LINE(answer);
}
`

const constraints =
`void Constraints() {
    CONS(valueOf(N).isBetween(1, 1000);
    CONS(eachElementOf(A).isBetween(1, 10));
}`

const testCases =
`void TestCases() {
    CASE(N = 1, A = {10});
    CASE(N = 100, randomArray());
}`

const FeatureList = [
  {
    title: 'Define input/output formats',
    description: (
      <CodeBlock language="cpp">
        {ioFormats}
      </CodeBlock>
    ),
  },
  {
    title: 'Define constraints',
    description: (
      <CodeBlock language="cpp">
        {constraints}
      </CodeBlock>
    ),
  },
  {
    title: 'Define test cases',
    description: (
      <CodeBlock language="cpp">
        {testCases}
      </CodeBlock>
    ),
  },
];

function Feature({Svg, title, description}) {
  return (
    <div className={clsx('col col--4')}>
      <div className="padding-horiz--md">
        <Heading as="h3">{title}</Heading>
        <p>{description}</p>
      </div>
    </div>
  );
}

export default function HomepageFeatures() {
  return (
    <section className={styles.features}>
      <div className="container">
        <div className="row">
          {FeatureList.map((props, idx) => (
            <Feature key={idx} {...props} />
          ))}
        </div>
      </div>
    </section>
  );
}
